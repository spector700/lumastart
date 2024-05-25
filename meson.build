project('lumastart', 'cpp',
  version : '0.2',
  default_options : ['buildtype=release', 'warning_level=3', 'cpp_std=c++23', 'default_library=static'])

globber = run_command('sh', '-c', 'find . -name "*.cpp" | sort', check: true)
src = globber.stdout().strip().split('\n')

deps = [
  dependency('gtk4-layer-shell-0'),
  dependency('gtkmm-4.0', version: '>= 4.6.0'),
]

gnome = import('gnome')
# Create a resource bundle to embed the files into the binary
resources = gnome.compile_resources('resources',
                                    'res/resources.gresource.xml',
                                    source_dir: './res/')

executable('lumastart', src,
          resources,
          dependencies : [ deps ],
          install : true)
