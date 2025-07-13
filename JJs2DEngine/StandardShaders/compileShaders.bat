%VULKAN_SDK%\Bin\glslc.exe UILayerShader.vert --target-env=vulkan -O -o UILayerShader.vert.spv
%VULKAN_SDK%\Bin\glslc.exe StandardFragmentShader.frag --target-env=vulkan -O -o StandardFragmentShader.frag.spv
%VULKAN_SDK%\Bin\glslc.exe UILayerShader.vert --target-env=vulkan -O0 -o UILayerShader.debug.vert.spv
%VULKAN_SDK%\Bin\glslc.exe StandardFragmentShader.frag --target-env=vulkan -O0 -o StandardFragmentShader.debug.frag.spv

%VULKAN_SDK%\Bin\glslc.exe UILayerShader.vert --target-env=vulkan -S -O -o UILayerShader.vert.txt
%VULKAN_SDK%\Bin\glslc.exe StandardFragmentShader.frag --target-env=vulkan -S -O -o StandardFragmentShader.frag.txt
%VULKAN_SDK%\Bin\glslc.exe UILayerShader.vert --target-env=vulkan -S -O0 -o UILayerShader.debug.vert.txt
%VULKAN_SDK%\Bin\glslc.exe StandardFragmentShader.frag --target-env=vulkan -S -O0 -o StandardFragmentShader.debug.frag.txt

pause