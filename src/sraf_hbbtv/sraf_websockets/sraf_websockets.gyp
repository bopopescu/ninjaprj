# copyright (c), 2015, SERAPHIC Information Technology (Shanghai) Co., Ltd.
# All Right Reserved.
#
{
  'includes': [
    '../../build/common.gypi',
  ],
  'targets': [
    {
      'target_name': 'sraf_websockets',
      'type': 'shared_library',
      'cflags!': [
        '-Wno-format',
        '-O0',
        '-fvisibility=hidden',
      ],
      'cflags_cc': [
        '-Werror',
        '-Werror=unused-variable',
      ],
      'cflags!': [
          '-Xclang', '-load',
      ],
      'include_dirs': [
        '<(DEPTH)/sraf/include',
      ],
      'direct_dependent_settings': {
        'include_dirs': [
          '<(DEPTH)/sraf/include',
        ],
      },
      'sources': [
        'lib/base64-decode.c',
        'lib/handshake.c',
        'lib/libwebsockets.c',
        'lib/service.c',
        'lib/pollfd.c',
        'lib/output.c',
        'lib/parsers.c',
        'lib/context.c',
        'lib/alloc.c',
        'lib/header.c',
        'lib/client.c',
        'lib/client-handshake.c',
        'lib/client-parser.c',
        'lib/sha-1.c',
        'lib/lws-plat-unix.c',
        'lib/server.c',
        'lib/server-handshake.c',
      ],
   },
  {
      'target_name': 'sraf_websockets-test-echo',
      'type': 'executable',
      'cflags!': [
          '-Wno-format',
      '-O0',
      '-fvisibility=hidden',
      ],
      'cflags_cc': [
          '-Werror',
      '-Werror=unused-variable',
      ],
      'include_dirs': [
          '<(DEPTH)/sraf/include',
          '<(DEPTH)/sraf/sraf_websockets/lib',
      ],
      'direct_dependent_settings': {
          'include_dirs': [
              '<(DEPTH)/sraf/include',
          ],
      },
      'dependencies': [
          'sraf_websockets',
      ],

      'sources': [
          'test-server/test-echo.c',
      ],
  },
  {
      'target_name': 'sraf_websockets-test-server',
      'type': 'executable',
      'cflags!': [
          '-Wno-format',
      '-O0',
      '-fvisibility=hidden',
      ],
      'cflags_cc': [
          '-Werror',
      '-Werror=unused-variable',
      ],
      'include_dirs': [
          '<(DEPTH)/sraf/include',
          '<(DEPTH)/sraf/sraf_websockets/lib',
      ],
      'direct_dependent_settings': {
          'include_dirs': [
              '<(DEPTH)/sraf/include',
          ],
      },
      'dependencies': [
          'sraf_websockets',
      ],

      'sources': [
          'test-server/tstserver.cpp',
      ],
  },
  {
      'target_name': 'sraf_media_sync_test',
      'type': 'executable',
      'cflags!': [
          '-Wno-format',
      '-O0',
      '-fvisibility=hidden',
      ],
      'cflags_cc': [
          '-Werror',
      '-Werror=unused-variable',
      ],
      'include_dirs': [
          '<(DEPTH)/sraf_hbbtv/sraf_websockets/sockserver',
      ],
      'dependencies': [
          'sraf_websockets',
      ],

      'sources': [
          'sockserver/media_sync_server.cpp',
          'sockserver/websocketserver.cpp',
          'test-server/media_sync_test.cpp',
      ],
  },

 ],
}
