./build/gyp_chromium sraf_hbbtv/sraf_websockets/sraf_websockets.gyp
ninja -C out/Debug sraf_websockets  sraf_media_sync_test  -j1
#ninja -C out/Release sraf_websockets sraf_websockets-test-server sraf_media_sync_test sraf_forward_test
