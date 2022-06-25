@echo OFF

mypy --config-file python\mypy.ini python\drkcraft launcher\src launcher\scripts tools
