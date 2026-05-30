from setuptools import find_packages, setup

package_name = "pai_moveit_interface"

setup(
    name=package_name,
    version="0.0.0",
    packages=find_packages(exclude=["test"]),
    data_files=[
        ("share/ament_index/resource_index/packages", [f"resource/{package_name}"]),
        (f"share/{package_name}", ["package.xml"]),
    ],
    install_requires=["setuptools"],
    zip_safe=True,
    maintainer="Maintainer",
    maintainer_email="maintainer@example.com",
    description="Placeholder MoveIt 2 integration package.",
    license="TODO",
    tests_require=["pytest"],
    entry_points={
        "console_scripts": [
            "moveit_interface_node = pai_moveit_interface.moveit_interface_node:main",
        ],
    },
)
