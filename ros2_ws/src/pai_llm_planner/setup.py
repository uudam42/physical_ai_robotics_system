from setuptools import find_packages, setup

package_name = "pai_llm_planner"

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
    description="Placeholder natural language task planner.",
    license="TODO",
    tests_require=["pytest"],
    entry_points={
        "console_scripts": [
            "planner_node = pai_llm_planner.planner_node:main",
            "mock_llm_planner_node = pai_llm_planner.mock_llm_planner_node:main",
        ],
    },
)
