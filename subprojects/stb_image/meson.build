project('stb_image', 'cpp', default_options : ['cpp_std=c++17'])

stb_src = [
    'stb_image.cpp'
]
stb_inc = include_directories('stb-repo')
stb = static_library('stb', stb_src, include_directories : stb_inc)
stb_dep = declare_dependency(include_directories : stb_inc, link_with : stb)