# Flux Pro Engine — 8 Ball Pool Android Game Mod

Native library (`library.so`) for FluxProEngine8BP — a root-injected cheat/modification for Miniclip's 8 Ball Pool game with ESP, Auto Play, Auto Queue, Auto Aim, and Dear ImGui overlay.

## Build

```bash
export ANDROID_NDK_HOME=/path/to/ndk/24.0.8215888
export PATH=$ANDROID_NDK_HOME:$PATH
ndk-build
```

Output: `obj/local/arm64-v8a/library.so`

## Backup — Cleanup Files

### Cleanup 1 — File tidak perlu (first pass)
Beberapa file yang tidak diperlukan untuk build telah dihapus.
- **Backup**: [Download](https://tmpfiles.org/wCw2e9trUkwY/flux-pro-engine-cleaned-files-backup.zip)
- **Isi**: IDE artifacts, unused version catalog, prebuilt armeabi-v7a libs, test files, original asset files (ttf/png), xhook build artifacts, file backup lama.

### Cleanup 2 — APK infrastructure dihapus
Gradle, Java layer, AndroidManifest, dan resources dihapus karena hanya `library.so` yang diperlukan.
- **Backup**: [Download](https://tmpfiles.org/w6wjeAtgdQ7J/flux-pro-engine-apk-backup.zip)
- **Isi**: Root Gradle, App Gradle, Java source (MainActivity, TelegramLogoBase64), Android resources, AndroidManifest.
