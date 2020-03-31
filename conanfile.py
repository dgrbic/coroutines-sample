from conans import ConanFile, CMake 

class CoroTest(ConanFile):
    settings = "os", "compiler", "build_type", "arch"
    generators = "cmake"
    requires = "range-v3/0.10.0@ericniebler/stable"

    def configure(self):
        self.settings.compiler = "clang"
        self.settings.compiler.version = 9
        self.settings.compiler.libcxx = "libc++"
        self.settings.compiler.cppstd="20"
        return super().configure()

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()