project('lumastart', 'cpp',
  version : '0.1',
  default_options : ['warning_level=3', 'cpp_std=c++23'])

globber = run_command('sh', '-c', 'find . -name "*.cpp" | sort', check: true)
src = globber.stdout().strip().split('\n')

deps = [
  dependency('gtkmm-4.0', version: '>= 4.6.0'),
  dependency('gtk4'),
  dependency('glibmm-2.68'),
  dependency('cairomm-1.16'),
  dependency('pangomm-2.48')
]

gtk_layer_shell = dependency('gtk-layer-shell-0',
                             version : ['>=1.0.2'],
                             fallback : ['gtk4-layer-shell', 'gtk_layer_shell'],
                             default_options : ['vapi=false'])

# if gtk_layer_shell.found()
#     add_project_arguments('-DHAVE_GTK_LAYER_SHELL', language: 'cpp')
# endif

executable('lumastart', src,
          dependencies : [
            deps,
            gtk_layer_shell
            ],
           install : true)
