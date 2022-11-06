project ('Duckie Map Tools', 'cpp', default_options : ['cpp_std=c++17'])

cmake = import('cmake')

fmt = dependency('fmt')

glm = dependency('glm')

stb_image_sp = subproject('stb_image')
stb_image = stb_image_sp.get_variable('stb_dep')
# stb_image = dependency('stb_image')

DuckCSG_include = include_directories('DuckCSG')
DuckCSG_src = [
'DuckCSG/Brush.cpp',
'DuckCSG/duckpch.cpp',
'DuckCSG/Entity.cpp',
'DuckCSG/Log.cpp',
'DuckCSG/Main.cpp',
'DuckCSG/MapParser.cpp',
'DuckCSG/Plane.cpp',
'DuckCSG/Polygon.cpp'
]

DuckCSG = executable('duckcsg', DuckCSG_src, dependencies : [glm, stb_image, fmt])

run_target('run', command : 'scripts/run.sh')