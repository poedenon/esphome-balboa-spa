#include "balboaspa.h"

namespace esphome {
namespace balboa_spa {

static const char *TAG = "BalboaSpa.component";

// Optimized constants for better performance
static const uint32_t COMMUNICATION_TIMEOUT_MS = 10000;
static const uint32_t FILTER_SETTINGS_REQUEST_INTERVAL_MS = 60000;
static const uint32_t MINUTES_PER_DAY = 1440;

void BalboaSpa::setup() {
    input_queue.clear();
    output_queue.clear();
    // Initialize state tracking
    last_received_time = 0;
    last_filtersettings_request = 0;
    client_id = 0;
    send_command = 0x00;
    config_request_status = 0;
    faultlog_request_status = 0;
    filtersettings_request_status = 0;
    last_state_crc = 0;
    
    // Debug temperature scale initialization
    ESP_LOGD(TAG, "Setup - Initial spa_temp_scale: %d, esphome_temp_scale: %d", spa_temp_scale, esphome_temp_scale);
}

void BalboaSpa::update() {
    uint32_t now = millis();
    
    // Check communication timeout with optimized logic
    if (last_received_time > 0 && (now - last_received_time) > COMMUNICATION_TIMEOUT_MS) {
        ESP_LOGW(TAG, "No communication for %d seconds - marking as dead!", (now - last_received_time) / 1000);
        status_set_error("No Communication with Balboa Mainboard!");
        client_id = 0;
    } else if (status_has_error()) {
        status_clear_error();
    }

    // Optimized periodic filter settings request
    if (now - last_filtersettings_request > FILTER_SETTINGS_REQUEST_INTERVAL_MS) {
        if (client_id != 0) {
            output_queue.push(client_id);
            output_queue.push(0xBF);
            output_queue.push(0x22);
            output_queue.push(0x01);
            output_queue.push(0x00);
            output_queue.push(0x00);
            ESP_LOGD(TAG, "Requesting filter settings (periodic)");
            rs485_send();
        }
        last_filtersettings_request = now;
    }

    // Process available serial data
    while (available()) {
      read_serial();
    }

    // Run through listeners with null check
    if (!this->listeners_.empty()) {
      for (const auto &listener : this->listeners_) {
        if (listener) {
          listener(&spaState);
        }
      }
    }
}

float BalboaSpa::get_setup_priority() const { return esphome::setup_priority::LATE; }

SpaConfig BalboaSpa::get_current_config() { return spaConfig; }
SpaState* BalboaSpa::get_current_state() { return &spaState; }

void BalboaSpa::set_temp(float temp) {
    float target_temp = 0.0;

    // Optimized temperature validation
    if (esphome_temp_scale == TEMP_SCALE::C) {
        if (temp >= ESPHOME_BALBOASPA_MIN_TEMPERATURE_C && temp <= ESPHOME_BALBOASPA_MAX_TEMPERATURE_C) {
            target_temp = temp;
        } else {
            ESP_LOGW(TAG, "Invalid Celsius temperature: %.1f", temp);
            return;
        }
    } else if (esphome_temp_scale == TEMP_SCALE::F) {
        if (temp >= ESPHOME_BALBOASPA_MIN_TEMPERATURE_F && temp <= ESPHOME_BALBOASPA_MAX_TEMPERATURE_F) {
            target_temp = convert_f_to_c(temp);
        } else {
            ESP_LOGW(TAG, "Invalid Fahrenheit temperature: %.1f", temp);
            return;
        }
    } else {
        ESP_LOGW(TAG, "Invalid temperature scale: %d", esphome_temp_scale);
        return;
    }

    // Set target temperature based on spa scale
    if (spa_temp_scale == TEMP_SCALE::C) {
        target_temperature = target_temp * 2;
    } else if (spa_temp_scale == TEMP_SCALE::F) {
        target_temperature = convert_c_to_f(target_temp);
    } else {
        ESP_LOGW(TAG, "Spa temperature scale not set: %d", spa_temp_scale);
        return;
    }

    send_command = 0xff;
}

void BalboaSpa::set_highrange(bool high) {
    if (high != spaState.highrange) {
        ESP_LOGD(TAG, "Setting highrange: %d -> %d", spaState.highrange, high);
        send_command = 0x50;
    }
}

void BalboaSpa::set_hour(int hour) {
    if (hour >= 0 && hour <= 23) {
        target_hour = hour;
        send_command = 0x21;
    } else {
        ESP_LOGW(TAG, "Invalid hour: %d", hour);
    }
}

void BalboaSpa::set_minute(int minute) {
    if (minute >= 0 && minute <= 59) {
        target_minute = minute;
        send_command = 0x21;
    } else {
        ESP_LOGW(TAG, "Invalid minute: %d", minute);
    }
}

void BalboaSpa::toggle_light() {
    send_command = 0x11;
}

void BalboaSpa::toggle_jet1() {
    send_command = 0x04;
}

void BalboaSpa::toggle_jet2() {
    send_command = 0x05;
}

void BalboaSpa::toggle_jet3() {
    send_command = 0x06;
}

void BalboaSpa::toggle_jet4() {
    send_command = 0x07;
}

void BalboaSpa::toggle_blower() {
    send_command = 0x0C;
}

void BalboaSpa::read_serial() {
    if (!read_byte(&received_byte)) {
        return;
    }

    // Drop until SOF is seen
    if (input_queue.first() != 0x7E && received_byte != 0x7E) {
        input_queue.clear();
        return;
    }

    // Double SOF-marker, drop last one
    if (input_queue.size() >= 2 && input_queue[1] == 0x7E) {
        input_queue.pop();
        return;
    }

    input_queue.push(received_byte);

    // Complete package
    //if (received_byte == 0x7E && input_queue[0] == 0x7E && input_queue[1] != 0x7E) {
    if (received_byte == 0x7E && input_queue.size() > 2 && input_queue.size() >= input_queue[1] + 2) {

        if (input_queue.size() - 2 < input_queue[1]) {
            ESP_LOGD(TAG, "packet_size: %d, recv_size: %d", input_queue[1], input_queue.size());
            ESP_LOGD(TAG, "%s", "Packet incomplete!");
            input_queue.clear();
            return;
        }

        auto calculated_crc = this->crc8(input_queue, true);
        auto packet_crc = input_queue[input_queue[1]];
        if (calculated_crc != packet_crc) {
            ESP_LOGD(TAG, "CRC %d != Packet crc %d end=0x%X", calculated_crc, packet_crc, input_queue[input_queue[1] + 1]);
            input_queue.clear();
            return;
        }

        // Unregistered or yet in progress
        if (client_id == 0) {
            ESP_LOGD(TAG, "Spa/node/id: %s", "Unregistered");
            //if (input_queue[2] == 0xFE) print_msg(input_queue);
            print_msg(input_queue);
            // FE BF 02:got new client ID
            if (input_queue[2] == 0xFE && input_queue[4] == 0x02) {
                client_id = input_queue[5];
                if (client_id > 0x2F) client_id = 0x2F;
                ESP_LOGD(TAG, "Spa/node/id: Got ID: %d, acknowledging", client_id);
                ID_ack();
                ESP_LOGD(TAG, "Spa/node/id: %d", client_id);
            }

            // FE BF 00:Any new clients?
            if (input_queue[2] == 0xFE && input_queue[4] == 0x00) {
                ESP_LOGD(TAG, "Spa/node/id: %s", "Requesting ID");
                ID_request();
            }
        } else if (input_queue[2] == client_id && input_queue[4] == 0x06) { // we have an ID, do clever stuff
            // client_id BF 06:Ready to Send
            if (send_command == 0x21) {
                output_queue.push(client_id);
                output_queue.push(0xBF);
                output_queue.push(0x21);
                output_queue.push(target_hour);
                output_queue.push(target_minute);
            } else if (send_command == 0xff) {
                // 0xff marks dirty temperature for now
                output_queue.push(client_id);
                output_queue.push(0xBF);
                output_queue.push(0x20);
                output_queue.push(target_temperature);
            } else if (send_command == 0x00) {
                if (config_request_status == 0) { // Get configuration of the hot tub
                    output_queue.push(client_id);
                    output_queue.push(0xBF);
                    output_queue.push(0x22);
                    output_queue.push(0x00);
                    output_queue.push(0x00);
                    output_queue.push(0x01);
                    ESP_LOGD(TAG, "Spa/config/status: %s", "Getting config");
                    config_request_status = 1;
                } else if (faultlog_request_status == 0) { // Get the fault log
                    output_queue.push(client_id);
                    output_queue.push(0xBF);
                    output_queue.push(0x22);
                    output_queue.push(0x20);
                    output_queue.push(0xFF);
                    output_queue.push(0x00);
                    faultlog_request_status = 1;
                    ESP_LOGD(TAG, "Spa/debug/faultlog_request_status: %s", "requesting fault log, #1");
                } else if ((filtersettings_request_status == 0) && (faultlog_request_status == 2)) { // Get the filter cycles log once we have the faultlog
                    output_queue.push(client_id);
                    output_queue.push(0xBF);
                    output_queue.push(0x22);
                    output_queue.push(0x01);
                    output_queue.push(0x00);
                    output_queue.push(0x00);
                    ESP_LOGD(TAG, "Spa/debug/filtersettings_request_status: %s", "requesting filter settings, #1");
                    filtersettings_request_status = 1;
                } else {
                    // A Nothing to Send message is sent by a client immediately after a Clear to Send message if the client has no messages to send.
                    output_queue.push(client_id);
                    output_queue.push(0xBF);
                    output_queue.push(0x07);
                }
            } else {
                output_queue.push(client_id);
                output_queue.push(0xBF);
                output_queue.push(0x11);
                output_queue.push(send_command);
                output_queue.push(0x00);
            }

            rs485_send();
            send_command = 0x00;
        } else if (input_queue[2] == client_id && input_queue[4] == 0x2E) {
            if (last_state_crc != input_queue[input_queue[1]]) {
                decodeSettings();
            }
        } else if (input_queue[2] == client_id && input_queue[4] == 0x28) {
            if (last_state_crc != input_queue[input_queue[1]]) {
                decodeFault();
            }
        } else if (input_queue[2] == 0xFF && input_queue[4] == 0x13) { // FF AF 13:Status Update - Packet index offset 5
            if (last_state_crc != input_queue[input_queue[1]]) {
                decodeState();
            }
        } else if (input_queue[2] == client_id && input_queue[4] == 0x23) { // FF AF 23:Filter Cycle Message - Packet index offset 5
            if (last_state_crc != input_queue[input_queue[1]]) {
                ESP_LOGD(TAG, "Spa/debug/faultlog_request_status: %s", "decoding filter settings");
                decodeFilterSettings();
            }
        } else {
            // DEBUG for finding meaning
            //if (input_queue[2] & 0xFE || input_queue[2] == id)
            //print_msg(input_queue);
        }

        // Clean up queue
        input_queue.clear();
    }
    last_received_time = millis();
}

uint8_t BalboaSpa::crc8(CircularBuffer<uint8_t, 100> &data, bool ignore_delimiter) {
    unsigned long crc_value;
    int bit_index;
    uint8_t data_length = ignore_delimiter ? data.size() - 2 : data.size();

    crc_value = 0x02;
    for (size_t byte_index = ignore_delimiter; byte_index < data_length; byte_index++) {
        crc_value ^= data[byte_index];
        for (bit_index = 0; bit_index < 8; bit_index++) {
            if ((crc_value & 0x80) != 0) {
                crc_value <<= 1;
                crc_value ^= 0x7;
            } else {
                crc_value <<= 1;
            }
        }
    }
    return crc_value ^ 0x02;
}

void BalboaSpa::ID_request() {
    output_queue.push(0xFE);
    output_queue.push(0xBF);
    output_queue.push(0x01);
    output_queue.push(0x02);
    output_queue.push(0xF1);
    output_queue.push(0x73);

    rs485_send();
}

void BalboaSpa::ID_ack() {
    output_queue.push(client_id);
    output_queue.push(0xBF);
    output_queue.push(0x03);

    rs485_send();
}

void BalboaSpa::rs485_send() {
    // Add telegram length
    output_queue.unshift(output_queue.size() + 2);

    // Add CRC
    output_queue.push(crc8(output_queue, false));

    // Wrap telegram in SOF/EOF
    output_queue.unshift(0x7E);
    output_queue.push(0x7E);

    for (loop_index = 0; loop_index < output_queue.size(); loop_index++) {
        write(output_queue[loop_index]);
    }

    //print_msg(output_queue);

    flush();

    // DEBUG: print_msg(output_queue);
    output_queue.clear();
}

void BalboaSpa::print_msg(CircularBuffer<uint8_t, 100> &data) {
    std::stringstream debug_stream;
    //for (loop_index = 0; loop_index < (input_queue[1] + 2); loop_index++) {
    for (loop_index = 0; loop_index < data.size(); loop_index++) {
        received_byte = data[loop_index];
        if (received_byte < 0x0A) debug_stream << "0";
        debug_stream << std::hex << received_byte;
        debug_stream << " ";
    }
    yield();
}

void BalboaSpa::decodeSettings() {
    ESP_LOGD(TAG, "Spa/config/status: Got config");
    spaConfig.pump1 = input_queue[5] & 0x03;
    spaConfig.pump2 = (input_queue[5] & 0x0C) >> 2;
    spaConfig.pump3 = (input_queue[5] & 0x30) >> 4;
    spaConfig.pump4 = (input_queue[5] & 0xC0) >> 6;
    spaConfig.pump5 = (input_queue[6] & 0x03);
    spaConfig.pump6 = (input_queue[6] & 0xC0) >> 6;
    spaConfig.light1 = (input_queue[7] & 0x03);
    spaConfig.light2 = (input_queue[7] >> 2) & 0x03;
    spaConfig.circ = ((input_queue[8] & 0x80) != 0);
    spaConfig.blower = ((input_queue[8] & 0x03) != 0);
    spaConfig.mister = ((input_queue[9] & 0x30) != 0);
    spaConfig.aux1 = ((input_queue[9] & 0x01) != 0);
    spaConfig.aux2 = ((input_queue[9] & 0x02) != 0);
    spaConfig.temperature_scale = input_queue[3] & 0x01; //Read temperature scale - 0 -> Farenheit, 1-> Celcius
    ESP_LOGD(TAG, "Spa/config/pumps1: %d", spaConfig.pump1);
    ESP_LOGD(TAG, "Spa/config/pumps2: %d", spaConfig.pump2);
    ESP_LOGD(TAG, "Spa/config/pumps3: %d", spaConfig.pump3);
    ESP_LOGD(TAG, "Spa/config/pumps4: %d", spaConfig.pump4);
    ESP_LOGD(TAG, "Spa/config/pumps5: %d", spaConfig.pump5);
    ESP_LOGD(TAG, "Spa/config/pumps6: %d", spaConfig.pump6);
    ESP_LOGD(TAG, "Spa/config/light1: %d", spaConfig.light1);
    ESP_LOGD(TAG, "Spa/config/light2: %d", spaConfig.light2);
    ESP_LOGD(TAG, "Spa/config/circ: %d", spaConfig.circ);
    ESP_LOGD(TAG, "Spa/config/blower: %d", spaConfig.blower);
    ESP_LOGD(TAG, "Spa/config/mister: %d", spaConfig.mister);
    ESP_LOGD(TAG, "Spa/config/aux1: %d", spaConfig.aux1);
    ESP_LOGD(TAG, "Spa/config/aux2: %d", spaConfig.aux2);
    ESP_LOGD(TAG, "Spa/config/temperature_scale: %d", spaConfig.temperature_scale);
    config_request_status = 2;

    if (spa_temp_scale == TEMP_SCALE::UNDEFINED) {
        spa_temp_scale = static_cast<TEMP_SCALE>(spaConfig.temperature_scale);
    }
}

void BalboaSpa::decodeState() {

    // Debug temperature parsing
    ESP_LOGD(TAG, "Temperature parsing - spa_temp_scale: %d, esphome_temp_scale: %d", spa_temp_scale, esphome_temp_scale);
    ESP_LOGD(TAG, "Raw temperature bytes - target: 0x%02X, current: 0x%02X", input_queue[25], input_queue[7]);
    
    // 25:Flag Byte 20 - Set Temperature (Target)
    if (input_queue[25] != 0xFF) {  // Check for valid temperature value
        double temp_read = 0.0;

        if (spa_temp_scale == TEMP_SCALE::C) {
            temp_read = input_queue[25] / 2.0;
        } else if (spa_temp_scale == TEMP_SCALE::F) {
            temp_read = convert_f_to_c(input_queue[25]);
        } else {
            ESP_LOGW(TAG, "Unknown spa temperature scale: %d", spa_temp_scale);
            temp_read = input_queue[25] / 2.0; // Default to Celsius
        }
        
        // Validate temperature range
        if (temp_read >= ESPHOME_BALBOASPA_MIN_TEMPERATURE_C && temp_read <= ESPHOME_BALBOASPA_MAX_TEMPERATURE_C) {
            if (esphome_temp_scale == TEMP_SCALE::C) {
                spaState.target_temp = temp_read;
                ESP_LOGD(TAG, "Spa/temperature/target: %.2f C", temp_read);
            } else if (esphome_temp_scale == TEMP_SCALE::F) {
                spaState.target_temp = convert_c_to_f(temp_read);
                ESP_LOGD(TAG, "Spa/temperature/target: %.2f F", spaState.target_temp);
            }
        } else {
            ESP_LOGW(TAG, "Target temperature out of range: %.2f (raw: 0x%02X)", temp_read, input_queue[25]);
        }
    } else {
        ESP_LOGD(TAG, "Target temperature byte is 0xFF (no valid value)");
    }

    // 7:Flag Byte 2 - Actual temperature (Current)
    if (input_queue[7] != 0xFF) {  // Check for valid temperature value
        double temp_read = 0.0;

        if (spa_temp_scale == TEMP_SCALE::C) {
            temp_read = input_queue[7] / 2.0;
        } else if (spa_temp_scale == TEMP_SCALE::F) {
            temp_read = convert_f_to_c(input_queue[7]);
        } else {
            ESP_LOGW(TAG, "Unknown spa temperature scale: %d", spa_temp_scale);
            temp_read = input_queue[7] / 2.0; // Default to Celsius
        }
        
        // Validate temperature range
        if (temp_read >= ESPHOME_BALBOASPA_MIN_TEMPERATURE_C && temp_read <= ESPHOME_BALBOASPA_MAX_TEMPERATURE_C) {
            if (esphome_temp_scale == TEMP_SCALE::C) {
                spaState.current_temp = temp_read;
                ESP_LOGD(TAG, "Spa/temperature/current: %.2f C", temp_read);
            } else if (esphome_temp_scale == TEMP_SCALE::F) {
                spaState.current_temp = convert_c_to_f(temp_read);
                ESP_LOGD(TAG, "Spa/temperature/current: %.2f F", spaState.current_temp);
            }
        } else {
            ESP_LOGW(TAG, "Current temperature out of range: %.2f (raw: 0x%02X)", temp_read, input_queue[7]);
        }
    } else {
        ESP_LOGD(TAG, "Current temperature byte is 0xFF (no valid value)");
    }

    // 8:Flag Byte 3 Hour & 9:Flag Byte 4 Minute => Time

    target_hour = input_queue[8];
    target_minute = input_queue[9];

    if (target_hour != spaState.hour || target_minute != spaState.minutes) {
        // Do not trigger a new state for clock
        // newState = true;
        // ESP_LOGD(TAG, "Spa/time/state %s", s.c_str());
        spaState.hour = target_hour;
        spaState.minutes = target_minute;
    }

    spaState.rest_mode = input_queue[10];

    // 15:Flags Byte 10 / Heat status, Temp Range
    spaState.heat_state = bitRead(input_queue[15], 4);

    double spa_component_state = bitRead(input_queue[15], 2);
    if (spa_component_state != spaState.highrange) {
        ESP_LOGD(TAG, "Spa/highrange/state: %.0f", spa_component_state); //LOW
        spaState.highrange = spa_component_state;
    }

    // 16:Flags Byte 11
    spa_component_state = bitRead(input_queue[16], 1);
    if (spa_component_state != spaState.jet1) {
        ESP_LOGD(TAG, "Spa/jet_1/state: %.0f", spa_component_state);
        spaState.jet1 = spa_component_state;
    }

    spa_component_state = bitRead(input_queue[16], 3);
    if (spa_component_state != spaState.jet2) {
        ESP_LOGD(TAG, "Spa/jet_2/state: %.0f", spa_component_state);
        spaState.jet2 = spa_component_state;
    }

    spa_component_state = bitRead(input_queue[16], 5);
    if (spa_component_state != spaState.jet3) {
        ESP_LOGD(TAG, "Spa/jet_3/state: %.0f", spa_component_state);
        spaState.jet3 = spa_component_state;
    }

    spa_component_state = bitRead(input_queue[16], 7);
    if (spa_component_state != spaState.jet4)
    {
      ESP_LOGD(TAG, "Spa/jet_4/state: %.0f", spa_component_state);
      spaState.jet4 = spa_component_state;
    }

    // 18:Flags Byte 13
    spa_component_state = bitRead(input_queue[18], 1);
    if (spa_component_state != spaState.circulation) {
        ESP_LOGD(TAG, "Spa/circ/state: %.0f", spa_component_state);
        spaState.circulation = spa_component_state;
    }

    spa_component_state = bitRead(input_queue[18], 2);
    if (spa_component_state != spaState.blower) {
        ESP_LOGD(TAG, "Spa/blower/state: %.0f", spa_component_state);
        spaState.blower = spa_component_state;
    }

    spa_component_state = input_queue[19] == 0x03;
    // 19:Flags Byte 14
    if (spa_component_state != spaState.light) {
        ESP_LOGD(TAG, "Spa/light/state: %.0f", spa_component_state);
        spaState.light = spa_component_state;
    }

    // Store the raw status bytes for debugging
    last_status_byte_16 = input_queue[16];
    last_status_byte_17 = input_queue[17];
    last_status_byte_18 = input_queue[18];
    last_status_byte_19 = input_queue[19];
    ESP_LOGD(TAG, "Spa/debug/status_bytes: 16=0x%02X 17=0x%02X 18=0x%02X 19=0x%02X", 
             last_status_byte_16, last_status_byte_17, last_status_byte_18, last_status_byte_19);

    // Decode actual pump status from byte 16
    // Pump 1: bits 0-1 (0x03) - 0=off, 1=low, 2=high
    uint8_t pump1_status = input_queue[16] & 0x03;
    if (pump1_status != spaState.pump1) {
        ESP_LOGD(TAG, "Spa/pump1/actual_state: %d", pump1_status);
        spaState.pump1 = pump1_status;
    }
    
    // Pump 2: bits 3-4 (0x18) - 0=off, 8=on (bit 3 set)
    uint8_t pump2_status = (input_queue[16] & 0x18) >> 3;
    if (pump2_status != spaState.pump2) {
        ESP_LOGD(TAG, "Spa/pump2/actual_state: %d", pump2_status);
        spaState.pump2 = pump2_status;
    }
    
    // Pump 3: not used in your spa
    spaState.pump3 = 0;

    // Filter status tracking
    update_filter_status();

    // TODO: callback on newState

    last_state_crc = input_queue[input_queue[1]];
}

void BalboaSpa::decodeFilterSettings() {
    spaFilterSettings.filter1_hour = input_queue[5];
    spaFilterSettings.filter1_minute = input_queue[6];
    spaFilterSettings.filter1_duration_hour = input_queue[7];
    spaFilterSettings.filter1_duration_minute = input_queue[8];
    
    // Always trust the spa response for filter2_enable state
    spaFilterSettings.filter2_enable = bitRead(input_queue[9], 7);
    
    spaFilterSettings.filter2_hour = input_queue[9] & 0x1F; // Clear bit 7, keep bits 0-4 for hour
    spaFilterSettings.filter2_minute = input_queue[10];
    spaFilterSettings.filter2_duration_hour = input_queue[11];
    spaFilterSettings.filter2_duration_minute = input_queue[12];

    //Filter 1 time conversion
    static PROGMEM const char *format_string = R"({"start":"%.2i:%.2i","duration":"%.2i:%.2i"} )";
    const auto payload_length = std::snprintf(nullptr, 0, format_string, spaFilterSettings.filter1_hour, spaFilterSettings.filter1_minute, spaFilterSettings.filter1_duration_hour, spaFilterSettings.filter1_duration_minute);

    char filter_payload[payload_length + 1];
    std::memset(filter_payload, 0, payload_length + 1);
    std::snprintf(filter_payload, payload_length + 1, format_string, spaFilterSettings.filter1_hour, spaFilterSettings.filter1_minute, spaFilterSettings.filter1_duration_hour, spaFilterSettings.filter1_duration_minute);
    ESP_LOGD(TAG, "Spa/filter1/state: %s", filter_payload);

    //Filter 2 time conversion
    ESP_LOGD(TAG, "Spa/filter2_enabled/state: %s", spaFilterSettings.filter2_enable == 1 ? STRON : STROFF);
    std::snprintf(filter_payload, payload_length + 1, format_string, spaFilterSettings.filter2_hour, spaFilterSettings.filter2_minute, spaFilterSettings.filter2_duration_hour, spaFilterSettings.filter2_duration_minute);
    ESP_LOGD(TAG, "Spa/filter2/state: %s", filter_payload);

    filtersettings_request_status = 2;
}

void BalboaSpa::decodeFault() {
    spaFaultLog.total_entries = input_queue[5];
    spaFaultLog.current_entry = input_queue[6];
    spaFaultLog.fault_code = input_queue[7];
    switch (spaFaultLog.fault_code) { // this is a inelegant way to do it, a lookup table would be better
        case 15:
            spaFaultLog.fault_message = "Sensors are out of sync";
            break;
        case 16:
            spaFaultLog.fault_message = "The water flow is low";
            break;
        case 17:
            spaFaultLog.fault_message = "The water flow has failed";
            break;
        case 18:
            spaFaultLog.fault_message = "The settings have been reset";
            break;
        case 19:
            spaFaultLog.fault_message = "Priming Mode";
            break;
        case 20:
            spaFaultLog.fault_message = "The clock has failed";
            break;
        case 21:
            spaFaultLog.fault_message = "The settings have been reset";
            break;
        case 22:
            spaFaultLog.fault_message = "Program memory failure";
            break;
        case 26:
            spaFaultLog.fault_message = "Sensors are out of sync -- Call for service";
            break;
        case 27:
            spaFaultLog.fault_message = "The heater is dry";
            break;
        case 28:
            spaFaultLog.fault_message = "The heater may be dry";
            break;
        case 29:
            spaFaultLog.fault_message = "The water is too hot";
            break;
        case 30:
            spaFaultLog.fault_message = "The heater is too hot";
            break;
        case 31:
            spaFaultLog.fault_message = "Sensor A Fault";
            break;
        case 32:
            spaFaultLog.fault_message = "Sensor B Fault";
            break;
        case 34:
            spaFaultLog.fault_message = "A pump may be stuck on";
            break;
        case 35:
            spaFaultLog.fault_message = "Hot fault";
            break;
        case 36:
            spaFaultLog.fault_message = "The GFCI test failed";
            break;
        case 37:
            spaFaultLog.fault_message = "Standby Mode (Hold Mode)";
            break;
        default:
            spaFaultLog.fault_message = "Unknown error";
            break;
    }
    spaFaultLog.days_ago = input_queue[8];
    spaFaultLog.hour = input_queue[9];
    spaFaultLog.minutes = input_queue[10];
    ESP_LOGD(TAG, "Spa/fault/Entries: %d", spaFaultLog.total_entries);
    ESP_LOGD(TAG, "Spa/fault/Entry: %d", spaFaultLog.current_entry);
    ESP_LOGD(TAG, "Spa/fault/Code: %d", spaFaultLog.fault_code);
    ESP_LOGD(TAG, "Spa/fault/Message: %s", spaFaultLog.fault_message.c_str());
    ESP_LOGD(TAG, "Spa/fault/DaysAgo: %d", spaFaultLog.days_ago);
    ESP_LOGD(TAG, "Spa/fault/Hours: %d", spaFaultLog.hour);
    ESP_LOGD(TAG, "Spa/fault/Minutes: %d", spaFaultLog.minutes);
    faultlog_request_status = 2;
    //ESP_LOGD(TAG, "Spa/debug/faultlog_request_status: have the faultlog, #2");
}

bool BalboaSpa::is_communicating() {
    return client_id != 0;
}

void BalboaSpa::set_spa_temp_scale(TEMP_SCALE scale) {
    ESP_LOGD(TAG, "Setting spa temperature scale: %d", scale);
    spa_temp_scale = scale;
}

void BalboaSpa::set_esphome_temp_scale(TEMP_SCALE scale) {
    ESP_LOGD(TAG, "Setting ESPHome temperature scale: %d", scale);
    esphome_temp_scale = scale;
}

float BalboaSpa::convert_c_to_f(float c) {
    return (c * 9.0 / 5.0) + 32.0;
}

float BalboaSpa::convert_f_to_c(float f) {
    return (f - 32.0) * 5.0 / 9.0;
}

// Filter update methods
void BalboaSpa::set_filter1_schedule(uint8_t start_hour, uint8_t start_minute, uint8_t duration_hour, uint8_t duration_minute) {
    if (start_hour > 23 || start_minute > 55 || duration_hour > 23 || duration_minute > 55) {
        ESP_LOGW(TAG, "Invalid filter 1 schedule parameters");
        return;
    }
    // Validate 5-minute increments
    if (start_minute % 5 != 0 || duration_minute % 5 != 0) {
        ESP_LOGW(TAG, "Filter 1 schedule minutes must be in 5-minute increments");
        return;
    }
    
    // Store the new settings
    spaFilterSettings.filter1_hour = start_hour;
    spaFilterSettings.filter1_minute = start_minute;
    spaFilterSettings.filter1_duration_hour = duration_hour;
    spaFilterSettings.filter1_duration_minute = duration_minute;
    
    // Send filter settings update command
    if (client_id != 0) {
        output_queue.push(client_id);
        output_queue.push(0xBF);
        output_queue.push(0x22);
        output_queue.push(0x01);
        output_queue.push(start_hour);
        output_queue.push(start_minute);
        output_queue.push(duration_hour);
        output_queue.push(duration_minute);
        output_queue.push(spaFilterSettings.filter2_hour | (spaFilterSettings.filter2_enable << 7));
        output_queue.push(spaFilterSettings.filter2_minute);
        output_queue.push(spaFilterSettings.filter2_duration_hour);
        output_queue.push(spaFilterSettings.filter2_duration_minute);
        ESP_LOGD(TAG, "Updating filter 1 schedule: %02d:%02d for %02d:%02d", start_hour, start_minute, duration_hour, duration_minute);
        rs485_send();
    }
}

void BalboaSpa::set_filter2_schedule(uint8_t start_hour, uint8_t start_minute, uint8_t duration_hour, uint8_t duration_minute) {
    if (start_hour > 23 || start_minute > 55 || duration_hour > 23 || duration_minute > 55) {
        ESP_LOGW(TAG, "Invalid filter 2 schedule parameters");
        return;
    }
    // Validate 5-minute increments
    if (start_minute % 5 != 0 || duration_minute % 5 != 0) {
        ESP_LOGW(TAG, "Filter 2 schedule minutes must be in 5-minute increments");
        return;
    }
    
    // Store the new settings
    spaFilterSettings.filter2_hour = start_hour;
    spaFilterSettings.filter2_minute = start_minute;
    spaFilterSettings.filter2_duration_hour = duration_hour;
    spaFilterSettings.filter2_duration_minute = duration_minute;
    
    // Send filter settings update command
    if (client_id != 0) {
        output_queue.push(client_id);
        output_queue.push(0xBF);
        output_queue.push(0x22);
        output_queue.push(0x01);
        output_queue.push(spaFilterSettings.filter1_hour);
        output_queue.push(spaFilterSettings.filter1_minute);
        output_queue.push(spaFilterSettings.filter1_duration_hour);
        output_queue.push(spaFilterSettings.filter1_duration_minute);
        output_queue.push(start_hour | (spaFilterSettings.filter2_enable << 7));
        output_queue.push(start_minute);
        output_queue.push(duration_hour);
        output_queue.push(duration_minute);
        ESP_LOGD(TAG, "Updating filter 2 schedule: %02d:%02d for %02d:%02d", start_hour, start_minute, duration_hour, duration_minute);
        rs485_send();
    }
}



void BalboaSpa::request_filter_settings() {
    // Send filter configuration request according to protocol
    if (client_id != 0) {
        output_queue.push(client_id);
        output_queue.push(0xBF);
        output_queue.push(0x22);
        output_queue.push(0x01);
        output_queue.push(0x00);
        output_queue.push(0x00);
        ESP_LOGD(TAG, "Requesting filter settings from spa");
        rs485_send();
    }
}

void BalboaSpa::reset_filter_runtime(uint8_t filter_number) {
    if (filter_number == 1) {
        spaState.filter1_runtime_hours = 0;
        ESP_LOGD(TAG, "Reset filter 1 runtime hours");
    } else if (filter_number == 2) {
        spaState.filter2_runtime_hours = 0;
        ESP_LOGD(TAG, "Reset filter 2 runtime hours");
    } else {
        ESP_LOGW(TAG, "Invalid filter number for runtime reset: %d", filter_number);
    }
}

void BalboaSpa::reset_filter_cycles(uint8_t filter_number) {
    if (filter_number == 1) {
        spaState.filter1_cycles_completed = 0;
        ESP_LOGD(TAG, "Reset filter 1 cycles completed");
    } else if (filter_number == 2) {
        spaState.filter2_cycles_completed = 0;
        ESP_LOGD(TAG, "Reset filter 2 cycles completed");
    } else {
        ESP_LOGW(TAG, "Invalid filter number for cycles reset: %d", filter_number);
    }
}

uint32_t BalboaSpa::get_filter1_current_runtime_minutes() const {
    if (!spaState.filter1_running) {
        return 0;
    }
    uint32_t current_time = millis();
    uint32_t runtime_ms = current_time - spaState.filter1_last_start_time;
    return runtime_ms / 60000; // Convert to minutes
}

uint32_t BalboaSpa::get_filter2_current_runtime_minutes() const {
    if (!spaState.filter2_running) {
        return 0;
    }
    uint32_t current_time = millis();
    uint32_t runtime_ms = current_time - spaState.filter2_last_start_time;
    return runtime_ms / 60000; // Convert to minutes
}

void BalboaSpa::update_filter_status() {
    uint32_t current_time = millis();
    
    // Optimized filter schedule checking with early returns
    auto check_filter_schedule = [](uint8_t start_hour, uint8_t start_minute, 
                                   uint8_t duration_hour, uint8_t duration_minute,
                                   uint8_t current_hour, uint8_t current_minute) -> bool {
        // Early return if no duration set
        if (duration_hour == 0 && duration_minute == 0) {
            return false;
        }
        
        uint32_t current_minutes = current_hour * 60 + current_minute;
        uint32_t filter_start_minutes = start_hour * 60 + start_minute;
        uint32_t filter_duration_minutes = duration_hour * 60 + duration_minute;
        
        if (filter_duration_minutes == 0) {
            return false;
        }
        
        uint32_t end_minutes = filter_start_minutes + filter_duration_minutes;
        
        // Handle wrap around midnight
        if (end_minutes > MINUTES_PER_DAY) {
            return (current_minutes >= filter_start_minutes) || (current_minutes < (end_minutes - MINUTES_PER_DAY));
        } else {
            return (current_minutes >= filter_start_minutes) && (current_minutes < end_minutes);
        }
    };
    
    // Check filter schedules
    bool filter1_should_run = check_filter_schedule(
        spaFilterSettings.filter1_hour, spaFilterSettings.filter1_minute,
        spaFilterSettings.filter1_duration_hour, spaFilterSettings.filter1_duration_minute,
        spaState.hour, spaState.minutes
    );
    
    bool filter2_should_run = false;
    if (spaFilterSettings.filter2_enable) {
        filter2_should_run = check_filter_schedule(
            spaFilterSettings.filter2_hour, spaFilterSettings.filter2_minute,
            spaFilterSettings.filter2_duration_hour, spaFilterSettings.filter2_duration_minute,
            spaState.hour, spaState.minutes
        );
    }
    
    // Update filter 1 status
    if (filter1_should_run != spaState.filter1_running) {
        if (filter1_should_run) {
            // Filter 1 starting
            spaState.filter1_running = true;
            spaState.filter1_last_start_time = current_time;
            ESP_LOGD(TAG, "Filter 1 started running");
        } else {
            // Filter 1 stopping
            spaState.filter1_running = false;
            uint32_t runtime_ms = current_time - spaState.filter1_last_start_time;
            spaState.filter1_runtime_hours += runtime_ms / 3600000; // Convert to hours
            spaState.filter1_cycles_completed++;
            ESP_LOGD(TAG, "Filter 1 stopped running, total runtime: %d hours, cycles: %d", 
                     spaState.filter1_runtime_hours, spaState.filter1_cycles_completed);
        }
    }
    
    // Update filter 2 status
    if (filter2_should_run != spaState.filter2_running) {
        if (filter2_should_run) {
            // Filter 2 starting
            spaState.filter2_running = true;
            spaState.filter2_last_start_time = current_time;
            ESP_LOGD(TAG, "Filter 2 started running");
        } else {
            // Filter 2 stopping
            spaState.filter2_running = false;
            uint32_t runtime_ms = current_time - spaState.filter2_last_start_time;
            spaState.filter2_runtime_hours += runtime_ms / 3600000; // Convert to hours
            spaState.filter2_cycles_completed++;
            ESP_LOGD(TAG, "Filter 2 stopped running, total runtime: %d hours, cycles: %d", 
                     spaState.filter2_runtime_hours, spaState.filter2_cycles_completed);
        }
    }
}

}  // namespace balboa_spa
}  // namespace esphome
