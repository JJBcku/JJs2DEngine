%VULKAN_SDK%\Bin\glslc.exe BackgroundShader.vert --target-env=vulkan -O -o BackgroundShader.vert.spv
%VULKAN_SDK%\Bin\glslc.exe BackgroundShader.vert --target-env=vulkan -O0 -o BackgroundShader.debug.vert.spv
%VULKAN_SDK%\Bin\glslc.exe BackgroundShader.vert --target-env=vulkan -S -O -o BackgroundShader.vert.txt
%VULKAN_SDK%\Bin\glslc.exe BackgroundShader.vert --target-env=vulkan -S -O0 -o BackgroundShader.debug.vert.txt

%VULKAN_SDK%\Bin\glslc.exe UILayerShader.vert --target-env=vulkan -O -o UILayerShader.vert.spv
%VULKAN_SDK%\Bin\glslc.exe UILayerShader.vert --target-env=vulkan -O0 -o UILayerShader.debug.vert.spv
%VULKAN_SDK%\Bin\glslc.exe UILayerShader.vert --target-env=vulkan -S -O -o UILayerShader.vert.txt
%VULKAN_SDK%\Bin\glslc.exe UILayerShader.vert --target-env=vulkan -S -O0 -o UILayerShader.debug.vert.txt

%VULKAN_SDK%\Bin\glslc.exe SharedShader.frag --target-env=vulkan -O -o SharedShader.frag.spv
%VULKAN_SDK%\Bin\glslc.exe SharedShader.frag --target-env=vulkan -O0 -o SharedShader.debug.frag.spv
%VULKAN_SDK%\Bin\glslc.exe SharedShader.frag --target-env=vulkan -S -O -o SharedShader.frag.txt
%VULKAN_SDK%\Bin\glslc.exe SharedShader.frag --target-env=vulkan -S -O0 -o SharedShader.debug.frag.txt

%VULKAN_SDK%\Bin\glslc.exe GammaCorrectionShader.vert --target-env=vulkan -O -o GammaCorrectionShader.vert.spv
%VULKAN_SDK%\Bin\glslc.exe GammaCorrectionShader.frag --target-env=vulkan -O -o GammaCorrectionShader.frag.spv
%VULKAN_SDK%\Bin\glslc.exe GammaCorrectionShader.vert --target-env=vulkan -O0 -o GammaCorrectionShader.debug.vert.spv
%VULKAN_SDK%\Bin\glslc.exe GammaCorrectionShader.frag --target-env=vulkan -O0 -o GammaCorrectionShader.debug.frag.spv

%VULKAN_SDK%\Bin\glslc.exe GammaCorrectionShader.vert --target-env=vulkan -S -O -o GammaCorrectionShader.vert.txt
%VULKAN_SDK%\Bin\glslc.exe GammaCorrectionShader.frag --target-env=vulkan -S -O -o GammaCorrectionShader.frag.txt
%VULKAN_SDK%\Bin\glslc.exe GammaCorrectionShader.vert --target-env=vulkan -S -O0 -o GammaCorrectionShader.debug.vert.txt
%VULKAN_SDK%\Bin\glslc.exe GammaCorrectionShader.frag --target-env=vulkan -S -O0 -o GammaCorrectionShader.debug.frag.txt

%VULKAN_SDK%\Bin\glslc.exe WorldLayerShader.vert --target-env=vulkan -O -o WorldLayerShader.vert.spv
%VULKAN_SDK%\Bin\glslc.exe WorldLayerShader.vert --target-env=vulkan -O0 -o WorldLayerShader.debug.vert.spv
%VULKAN_SDK%\Bin\glslc.exe WorldLayerShader.vert --target-env=vulkan -S -O -o WorldLayerShader.vert.txt
%VULKAN_SDK%\Bin\glslc.exe WorldLayerShader.vert --target-env=vulkan -S -O0 -o WorldLayerShader.debug.vert.txt

pause