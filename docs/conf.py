
DOXYFILE = 'Doxyfile-mcss'
MAIN_PROJECT_URL: ""

LINKS_NAVBAR1 = [

  ("<a href=>ODE</a>", #("<a href=\"md_pages_tutorials.html\">Tutorials</a>",
   [
     ("<a href=\"md_pages_ode_tutorial1.html\">Explicit Time Integration</a>", ),
   ]),

  ("<a href=>ROM</a>", #("<a href=\"md_pages_tutorials.html\">Tutorials</a>",
   [
     ("<a href=\"md_pages_rom_tutorial1.html\">Linear Decoder</a>", ),
     ("<a href=\"md_pages_rom_tutorial2.html\">Default Galerkin explicit</a>", ),
     ("<a href=\"md_pages_rom_tutorial3.html\">LSPG ROM of the shallow water equations</a>", )
   ])

  # # demos
  # ("<a href=>Demos</a>", #("<a href=\"md_pages_examples.html\">Full Examples</a>",
  #  [
  #    ("<a href=\"md_pages_examples_example1.html\">Example1</a>", )
  #  ]),

  # ('Classes', 'annotated', []),
  # ('Namespaces', 'namespaces', [])
]

LINKS_NAVBAR2 = []
#   ('Classes', 'annotated', []),
#   ('Namespaces', 'namespaces', [])
# ]

PLUGINS = ['m.htmlsanity', 'm.math', 'm.code', 'm.components', 'm.dot', 'm.images']

SHOW_UNDOCUMENTED = "YES"

FAVICON = 'favicon.ico'

# STYLESHEETS = [
#     'https://fonts.googleapis.com/css?family=Libre+Baskerville:400,400i,700,700i%7CSource+Code+Pro:400,400i,600',
#     '../css/m-light+documentation.compiled.css'
# ]
# THEME_COLOR = '#91cff4'
# FAVICON = 'favicon-light.png'

# STYLESHEETS = [
#     'https://fonts.googleapis.com/css?family=Source+Sans+Pro:400,400i,600,600i%7CSource+Code+Pro:400,400i,600',
#     '../css/m-dark+documentation.compiled.css'
# ]
# THEME_COLOR = '#cb4b16'
# FAVICON = 'favicon-dark.png'
