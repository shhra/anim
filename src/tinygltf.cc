// This tiny gltf should be included in only one .cc file.
// In this case the model loading will handle everything here.
// This way, the application will get the gltf model

#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <tiny_gltf.h>
