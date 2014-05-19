project "containos_unittest"
	uuid "135746F6-9F4A-4289-A0B8-B7391F959B1D"
	kind "ConsoleApp"
	language "C++"
	files {
	    "include/containos/*.h",
	    "include/containos/*.inl",
		"tests/*.h",
		"tests/*.cpp",
	}
	includedirs { "include", "../unitos/include" }
	links { "unitos" }
	objdir( "build/" .. _ACTION )
	location( "build/" .. _ACTION )
	targetdir( "build/" .. _ACTION )
	targetname "%{prj.name}_%{cfg.platform}"

	flags { "FatalWarnings", "NoBufferSecurityCheck", "NoEditAndContinue", "NoIncrementalLink", "NoPCH", "NoRTTI" }
	warnings "Extra"

	configuration "Debug"
		defines { "DEBUG" }
		flags { "Symbols" }
		targetsuffix "_d"

	configuration "Release"
		defines { "NDEBUG" }
		flags { "NoRuntimeChecks" }
		optimize "Speed"
		targetsuffix "_r"

	configuration { "vs*"}
		postbuildcommands { "\"$(TargetPath)\"" }
