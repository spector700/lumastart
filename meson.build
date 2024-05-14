project('lumastart', 'cpp',
  version : '0.1',
  default_options : ['buildtype=release', 'warning_level=3', 'cpp_std=c++23', 'default_library=static'])

globber = run_command('sh', '-c', 'find . -name "*.cpp" | sort', check: true)
src = globber.stdout().strip().split('\n')

deps = [
  dependency('gtk4-layer-shell-0'),
  dependency('gtkmm-4.0', version: '>= 4.6.0'),
  dependency('glibmm-2.68'),
  dependency('giomm-2.68')
  # dependency('cairomm-1.16'),
  # dependency('pangomm-2.48')
]


executable('lumastart', src,
          dependencies : [ deps ],
           install : true)
