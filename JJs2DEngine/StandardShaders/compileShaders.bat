%VULKAN_SDK%\Bin\glslc.exe UILayerShader.vert -O -o UILayerShader.vert.spv
%VULKAN_SDK%\Bin\glslc.exe StandardFragmentShader.frag -O -o StandardFragmentShader.frag.spv
%VULKAN_SDK%\Bin\glslc.exe UILayerShader.vert -O0 -o UILayerShader.debug.vert.spv
%VULKAN_SDK%\Bin\glslc.exe StandardFragmentShader.frag -O0 -o StandardFragmentShader.debug.frag.spv

%VULKAN_SDK%\Bin\glslc.exe UILayerShader.vert -S -O -o UILayerShader.vert.txt
%VULKAN_SDK%\Bin\glslc.exe StandardFragmentShader.frag -S -O -o StandardFragmentShader.frag.txt
%VULKAN_SDK%\Bin\glslc.exe UILayerShader.vert -S -O0 -o UILayerShader.debug.vert.txt
%VULKAN_SDK%\Bin\glslc.exe StandardFragmentShader.frag -S -O0 -o StandardFragmentShader.debug.frag.txt

pause