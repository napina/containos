project "containos"
	uuid "C9A550D0-4F61-435F-935F-AB95E86B3D53"
	kind "StaticLib"
	language "C++"
	files { 
		"include/**.h",
		"include/**.inl",
		"src/*.h",
		"src/*.cpp",
	}
	includedirs { "include", "../reflectos/include" }
	objdir( "build/" .. _ACTION )
	location( "build/" .. _ACTION )
	targetdir "lib"
	targetname "%{prj.name}_%{cfg.platform}"

	flags { "FatalWarnings", "NoBufferSecurityCheck", "NoEditAndContinue", "NoIncrementalLink", "NoPCH", "NoRTTI" }
	warnings "Extra"

	configuration "Debug"
		defines { "DEBUG", "_ITERATOR_DEBUG_LEVEL=0" }
		flags { "Symbols" }
		targetsuffix "_d"

	configuration "Release"
		defines { "NDEBUG" }
		flags { "NoRuntimeChecks" }
		optimize "Speed"
		targetsuffix "_r"
