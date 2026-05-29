APP_ABI := arm64-v8a
APP_STL := c++_static
APP_OPTIM := release
# Android 5.0+ এর জন্য PIE সব সময় ট্রু থাকে, তাই এটি রাখা ভালো
APP_PIE := true
# আপনার প্রোজেক্ট অনুযায়ী প্লাটফর্ম ২১ বা তার উপরে রাখা নিরাপদ
APP_PLATFORM := android-21
# C++20 সাপোর্ট নিশ্চিত করতে এবং এরর কমাতে কিছু ফ্ল্যাগ যোগ করা হয়েছে
APP_CPPFLAGS := -std=c++20 -frtti -fexceptions -fpermissive
APP_BUILD_SCRIPT := Android.mk
