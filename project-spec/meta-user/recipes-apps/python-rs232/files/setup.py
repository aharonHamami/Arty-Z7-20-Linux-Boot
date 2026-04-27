from setuptools import setup, find_packages

setup(
    name="python-rs232",
    version="1.0",
    packages=find_packages("."),
    install_requires=[],
    entry_points={
        "console_scripts": [
            "python-rs232 = python_rs232.main:main"
        ]
    }
)
