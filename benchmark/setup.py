from setuptools import Extension, setup 

import os 

this_dir = os.path.dirname(__file__)


setup(
    name="ascii",
    version="0.1.0",
    ext_modules=[
        Extension("ascii", [os.path.join(this_dir, "asciimodule.c")], 
                  include_dirs=[os.path.dirname(this_dir)])
    ]
)