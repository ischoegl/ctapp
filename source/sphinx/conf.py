# Configuration file for the Sphinx documentation builder.
#
# For the full list of built-in configuration values, see the documentation:
# https://www.sphinx-doc.org/en/master/usage/configuration.html

# -- Project information -----------------------------------------------------
# https://www.sphinx-doc.org/en/master/usage/configuration.html#project-information

import cantera as ct
import ctapp

project = 'ctapp'
copyright = '2023, Ingmar Schoegl'
author = ctapp.__author__
release = ctapp.__version__



# -- General configuration ---------------------------------------------------
# https://www.sphinx-doc.org/en/master/usage/configuration.html#general-configuration

extensions = ['sphinx.ext.autodoc', 'breathe']

templates_path = ['_templates']
exclude_patterns = []


# -- Options for HTML output -------------------------------------------------
# https://www.sphinx-doc.org/en/master/usage/configuration.html#options-for-html-output

html_theme = "pydata_sphinx_theme"
html_static_path = ['_static']


# -- Options for breathe -----------------------------------------------------

breathe_projects = {"ctapp": "../../build/docs/doxygen/xml/"}
breathe_default_project = "ctapp"
