%VULKAN_SDK%\Bin\glslc.exe BackgroundShader.vert --target-env=vulkan -O -o BackgroundShader.vert.spv
%VULKAN_SDK%\Bin\glslc.exe BackgroundShader.vert --target-env=vulkan -O0 -o BackgroundShader.debug.vert.spv
%VULKAN_SDK%\Bin\glslc.exe BackgroundShader.vert --target-env=vulkan -S -O -o BackgroundShader.vert.txt
%VULKAN_SDK%\Bin\glslc.exe BackgroundShader.vert --target-env=vulkan -S -O0 -o BackgroundShader.debug.vert.txt

%VULKAN_SDK%\Bin\glslc.exe UILayerShader.vert --target-env=vulkan -O -o UILayerShader.vert.spv
%VULKAN_SDK%\Bin\glslc.exe StandardFragmentShader.frag --target-env=vulkan -O -o StandardFragmentShader.frag.spv
%VULKAN_SDK%\Bin\glslc.exe UILayerShader.vert --target-env=vulkan -O0 -o UILayerShader.debug.vert.spv
%VULKAN_SDK%\Bin\glslc.exe StandardFragmentShader.frag --target-env=vulkan -O0 -o StandardFragmentShader.debug.frag.spv

%VULKAN_SDK%\Bin\glslc.exe UILayerShader.vert --target-env=vulkan -S -O -o UILayerShader.vert.txt
%VULKAN_SDK%\Bin\glslc.exe StandardFragmentShader.frag --target-env=vulkan -S -O -o StandardFragmentShader.frag.txt
%VULKAN_SDK%\Bin\glslc.exe UILayerShader.vert --target-env=vulkan -S -O0 -o UILayerShader.debug.vert.txt
%VULKAN_SDK%\Bin\glslc.exe StandardFragmentShader.frag --target-env=vulkan -S -O0 -o StandardFragmentShader.debug.frag.txt

%VULKAN_SDK%\Bin\glslc.exe GammaCorrectionVertexShader.vert --target-env=vulkan -O -o GammaCorrectionVertexShader.vert.spv
%VULKAN_SDK%\Bin\glslc.exe GammaCorrectionFragmentShader.frag --target-env=vulkan -O -o GammaCorrectionFragmentShader.frag.spv
%VULKAN_SDK%\Bin\glslc.exe GammaCorrectionVertexShader.vert --target-env=vulkan -O0 -o GammaCorrectionVertexShader.debug.vert.spv
%VULKAN_SDK%\Bin\glslc.exe GammaCorrectionFragmentShader.frag --target-env=vulkan -O0 -o GammaCorrectionFragmentShader.debug.frag.spv

%VULKAN_SDK%\Bin\glslc.exe GammaCorrectionVertexShader.vert --target-env=vulkan -S -O -o GammaCorrectionVertexShader.vert.txt
%VULKAN_SDK%\Bin\glslc.exe GammaCorrectionFragmentShader.frag --target-env=vulkan -S -O -o GammaCorrectionFragmentShader.frag.txt
%VULKAN_SDK%\Bin\glslc.exe GammaCorrectionVertexShader.vert --target-env=vulkan -S -O0 -o GammaCorrectionVertexShader.debug.vert.txt
%VULKAN_SDK%\Bin\glslc.exe GammaCorrectionFragmentShader.frag --target-env=vulkan -S -O0 -o GammaCorrectionFragmentShader.debug.frag.txt

%VULKAN_SDK%\Bin\glslc.exe WorldLayerVertexShader.vert --target-env=vulkan -O -o WorldLayerVertexShader.vert.spv
%VULKAN_SDK%\Bin\glslc.exe WorldLayerVertexShader.vert --target-env=vulkan -O0 -o WorldLayerVertexShader.debug.vert.spv
%VULKAN_SDK%\Bin\glslc.exe WorldLayerVertexShader.vert --target-env=vulkan -S -O -o WorldLayerVertexShader.vert.txt
%VULKAN_SDK%\Bin\glslc.exe WorldLayerVertexShader.vert --target-env=vulkan -S -O0 -o WorldLayerVertexShader.debug.vert.txt

pause