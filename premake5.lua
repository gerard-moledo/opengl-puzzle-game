workspace "Puzzle"
   filename "Puzzle_Workspace"
   location("builds/" .. _ACTION)

   configurations { "Debug", "Release" }
   platforms { "Win32", "Win64" }

project "Puzzle"
   filename "Puzzle_Project"
   location("builds/" .. _ACTION .. "/project")

   kind "ConsoleApp"
   language "C++"
   targetdir "%{prj.location}/%{cfg.buildcfg}_%{cfg.platform}"
   targetname "program_vs2022"

   prebuildcommands { "copy /y ..\\..\\..\\Assets\\*.data Assets\\*.data",
                      "copy /y ..\\..\\..\\Assets\\*.vs Assets\\*.vs",
                      "copy /y ..\\..\\..\\Assets\\*.fs Assets\\*.fs",
                      "copy /y ..\\..\\..\\Assets\\*.png Assets\\*.png" }
   
   files { "include/*.hpp", "src/*.cpp", "D:/_Work/_Dev_Libs/glad-es/src/glad.c", "*.vs", "*.fs", "*.data" }

   links { "glfw3", "gdi32", "user32", "kernel32" }

   libdirs { "D:/_Work/_Dev_Libs/glfw-3.3.6.bin.WIN64/lib-vc2022"}

   includedirs { "include", "D:/_Work/_Dev_Libs/glad-es/include", "D:/_Work/_Dev_Libs/glfw-3.3.6.bin.WIN64/include",
                 "D:/_Work/_Dev_Libs/glm", "D:/_Work/_Dev_Libs/stb" }

   filter "configurations:Debug"
      defines { "DEBUG" }
      symbols "On"

   filter "configurations:Release"
      defines { "NDEBUG" }
      optimize "On"