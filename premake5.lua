solution "containos"
	location( "build/" .. _ACTION )
	configurations { "Debug", "Release" }
	platforms { "x32", "x64" }
	startproject "containos_unittest"

	include "containos_unittest.lua"
	include "../unitos/unitos.lua"