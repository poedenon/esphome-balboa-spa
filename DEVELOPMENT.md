# Development Guide

This guide explains how to develop and maintain the ESPHome Balboa Spa component while making it available for users via ESPHome's external components feature.

## 🏗️ Repository Structure

```
esphome-balboa-spa/
├── components/balboa_spa/          # Component source code
├── docs/                           # Documentation
├── .github/workflows/              # CI/CD automation
├── esphome-balboa-spa.yaml         # Example configuration
├── README.md                       # Main project overview
├── DEVELOPMENT.md                  # This file
└── CHANGELOG.md                    # Version history
```

## 🔄 Development Workflow

### **Branch Strategy**
- **`main`**: Stable releases, tagged versions
- **`develop`**: Development branch for new features
- **Feature branches**: For specific features/fixes

### **Development Process**

1. **Start Development**:
   ```bash
   git checkout develop
   # Make your changes
   git add .
   git commit -m "Add new feature"
   git push origin develop
   ```

2. **Test Changes**:
   ```yaml
   # In your test ESPHome config
   external_components:
     - source: github://poedenon/esphome-balboa-spa
       components: [ balboa_spa ]
       version: develop  # Test development branch
   ```

3. **Release to Main**:
   ```bash
   git checkout main
   git merge develop
   git push origin main
   git tag v1.1.0
   git push origin v1.1.0
   ```

## 📦 ESPHome External Components

### **For Users (Stable Version)**:
```yaml
external_components:
  - source: github://poedenon/esphome-balboa-spa
    components: [ balboa_spa ]
    version: v1.0.0  # Specific version
```

### **For Users (Latest Main)**:
```yaml
external_components:
  - source: github://poedenon/esphome-balboa-spa
    components: [ balboa_spa ]
    version: main  # Latest stable
```

### **For Development Testing**:
```yaml
external_components:
  - source: github://poedenon/esphome-balboa-spa
    components: [ balboa_spa ]
    version: develop  # Development branch
    refresh: 0s  # Always get latest
```

**Note**: ESPHome external components only support HTTPS URLs. The `github://` format is automatically converted to HTTPS by ESPHome.

## 🏷️ Version Management

### **Semantic Versioning**
- **v1.0.0**: Initial release
- **v1.0.1**: Bug fixes
- **v1.1.0**: New features
- **v2.0.0**: Breaking changes

### **Creating a Release**:
```bash
# Update version in manifest.yaml
# Update CHANGELOG.md
git add .
git commit -m "Prepare v1.1.0 release"
git tag v1.1.0
git push origin main
git push origin v1.1.0
```

## 🔧 Local Development Setup

### **1. Clone Repository**:
```bash
# For development (SSH - requires GitHub SSH key setup)
git clone git@github.com:poedenon/esphome-balboa-spa.git
cd esphome-balboa-spa

# OR for users (HTTPS - no setup required)
git clone https://github.com/poedenon/esphome-balboa-spa.git
cd esphome-balboa-spa
```

### **2. Create Development Branch**:
```bash
git checkout -b develop
git push -u origin develop
```

### **3. Test Your Changes**:
```bash
# Test compilation
esphome compile esphome-balboa-spa.yaml

# Test with external components
# Use version: develop in your test config
```

## 📋 Component Requirements

ESPHome looks for these files in your component:
- `components/balboa_spa/manifest.yaml` ✅
- `components/balboa_spa/__init__.py` ✅
- `components/balboa_spa/balboaspa.h` ✅
- `components/balboa_spa/balboaspa.cpp` ✅

## 🚀 Publishing Updates

### **Quick Update Process**:
1. Make changes on `develop` branch
2. Test thoroughly
3. Merge to `main`
4. Create version tag
5. Push to GitHub

### **Breaking Changes**:
1. Increment major version (v2.0.0)
2. Update documentation
3. Add migration guide
4. Notify users via GitHub releases

## 🔍 Testing Strategy

### **Local Testing**:
```bash
# Compile test
esphome compile esphome-balboa-spa.yaml

# Run CI checks locally
# Check documentation
# Test with different ESP32 boards
```

### **User Testing**:
- Use `develop` branch for beta testing
- Create GitHub releases for stable versions
- Monitor GitHub issues for feedback

## 📚 Documentation Updates

When making changes:
1. Update relevant documentation in `docs/`
2. Update `docs/README.md` if adding new sections
3. Update main `README.md` if needed
4. Update `CHANGELOG.md` for releases

## 🤝 Contributing Workflow

### **For Contributors**:
1. Fork the repository
2. Create feature branch
3. Make changes
4. Test thoroughly
5. Submit pull request

### **For You (Maintainer)**:
1. Review pull requests
2. Test changes
3. Merge to `develop`
4. Test on `develop`
5. Merge to `main` when ready

## 🎯 Best Practices

### **Code Quality**:
- Follow ESPHome coding standards
- Add proper error handling
- Include debug logging
- Test with multiple spa models

### **Documentation**:
- Keep documentation up to date
- Include working examples
- Provide troubleshooting guides
- Use clear, concise language

### **Version Control**:
- Use descriptive commit messages
- Tag releases properly
- Keep `develop` and `main` in sync
- Use feature branches for major changes

## 🔗 Useful Commands

### **Git Workflow**:
```bash
# Check status
git status

# Switch branches
git checkout develop
git checkout main

# Create new feature branch
git checkout -b feature/new-feature

# Update from remote
git pull origin main
git pull origin develop

# Push changes
git push origin develop
git push origin main
```

### **ESPHome Testing**:
```bash
# Compile configuration
esphome compile esphome-balboa-spa.yaml

# Validate configuration
esphome validate esphome-balboa-spa.yaml

# Upload to device
esphome upload esphome-balboa-spa.yaml
```

## 📞 Support

- **GitHub Issues**: For bug reports and feature requests
- **GitHub Discussions**: For questions and community support
- **Documentation**: Check `docs/` for detailed guides

---

This workflow ensures your component is always available to users while allowing you to develop and maintain it effectively. 