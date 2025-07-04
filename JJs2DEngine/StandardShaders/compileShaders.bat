%VULKAN_SDK%\Bin\glslc.exe UILayerShader.vert -O -o UILayerShader.vert.spv
%VULKAN_SDK%\Bin\glslc.exe StandardShader.frag -O -o StandardShader.frag.spv
%VULKAN_SDK%\Bin\glslc.exe UILayerShader.vert -O0 -o UILayerShader.debug.vert.spv
%VULKAN_SDK%\Bin\glslc.exe StandardShader.frag -O0 -o StandardShader.debug.frag.spv

%VULKAN_SDK%\Bin\glslc.exe UILayerShader.vert -S -O -o UILayerShader.vert.txt
%VULKAN_SDK%\Bin\glslc.exe StandardShader.frag -S -O -o StandardShader.frag.txt
%VULKAN_SDK%\Bin\glslc.exe UILayerShader.vert -S -O0 -o UILayerShader.debug.vert.txt
%VULKAN_SDK%\Bin\glslc.exe StandardShader.frag -S -O0 -o StandardShader.debug.frag.txt

pause