from setuptools import find_packages, setup

package_name = "pai_robot_actions"

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
    description="Placeholder robot manipulation action primitives.",
    license="TODO",
    tests_require=["pytest"],
    entry_points={
        "console_scripts": [
            "robot_actions_node = pai_robot_actions.robot_actions_node:main",
        ],
    },
)
