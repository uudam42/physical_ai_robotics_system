from setuptools import find_packages, setup

package_name = "pai_bt_executor"

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
    description="Placeholder BehaviorTree.CPP execution package.",
    license="TODO",
    tests_require=["pytest"],
    entry_points={
        "console_scripts": [
            "bt_executor_node = pai_bt_executor.bt_executor_node:main",
            "mock_bt_executor_node = pai_bt_executor.mock_bt_executor_node:main",
        ],
    },
)
