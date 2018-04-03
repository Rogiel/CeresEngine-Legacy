add_executable(XYZ.Sandbox Source/main2.cpp)
target_include_directories(XYZ.Sandbox
        PUBLIC ${SRC_DIR}
)

target_link_libraries(XYZ.Sandbox
        PUBLIC XYZ.Engine
        PUBLIC XYZ.Terrain.Noise
        PUBLIC XYZ.Terrain.Manager.Quadtree
        
        PRIVATE XYZ.Resource.Locator.Bundle
        
        PRIVATE XYZ.Graphics.Renderer.OpenGL
        PRIVATE XYZ.Graphics.Mesh.Obj
        PRIVATE XYZ.Graphics.Texture.Stbi
        PRIVATE XYZ.Graphics.Window.GLFW
        
        PRIVATE XYZ.Audio.OpenAL
        PRIVATE XYZ.Audio.Loader.OggVorbis
        
        PRIVATE XYZ.Input.Keyboard.GLFW
        PRIVATE XYZ.Input.Mouse.GLFW
        
        PRIVATE XYZ.Scene.Manager.Octree
        
        PRIVATE "-framework CoreFoundation"
        PRIVATE "-framework AudioToolbox"
)
