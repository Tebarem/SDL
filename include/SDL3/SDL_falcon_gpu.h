/*
  Falcon external-texture import for SDL_GPU.

  This private SDL extension normalizes OS-native CEF frame handles at the
  SDL_GPU boundary. Platform drivers own the import and GPU copy.
*/

#ifndef SDL_falcon_gpu_h_
#define SDL_falcon_gpu_h_

#include <SDL3/SDL_gpu.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define SDL_FALCON_EXTERNAL_TEXTURE_VERSION 1u
#define SDL_FALCON_EXTERNAL_TEXTURE_MAX_PLANES 4u
#define SDL_FALCON_GPU_PATCH_REVISION 2u

#if defined(SDL_FALCON_GPU_BUILD) && defined(_WIN32)
#define SDL_FALCON_GPU_API __declspec(dllexport)
#elif defined(_WIN32)
#define SDL_FALCON_GPU_API __declspec(dllimport)
#elif defined(__GNUC__)
#define SDL_FALCON_GPU_API __attribute__((visibility("default")))
#else
#define SDL_FALCON_GPU_API
#endif

typedef enum SDL_FalconExternalTextureKind {
    SDL_FALCON_EXTERNAL_TEXTURE_D3D12_SHARED_HANDLE = 1,
    SDL_FALCON_EXTERNAL_TEXTURE_DMA_BUF = 2,
    SDL_FALCON_EXTERNAL_TEXTURE_IOSURFACE = 3,
    SDL_FALCON_EXTERNAL_TEXTURE_ANDROID_HARDWARE_BUFFER = 4
} SDL_FalconExternalTextureKind;

typedef struct SDL_FalconExternalTexturePlane {
    intptr_t handle; /* DMA-BUF file descriptor */
    uint64_t size;
    uint64_t offset;
    uint32_t stride;
    uint32_t reserved;
} SDL_FalconExternalTexturePlane;

typedef struct SDL_FalconExternalTexture {
    uint32_t struct_size;
    uint32_t version;
    uint32_t kind;
    uint32_t plane_count;
    uint32_t width;
    uint32_t height;
    SDL_GPUTextureFormat format;
    uint32_t reserved;
    uint64_t modifier;
    union {
        void *handle; /* Win32 shared handle, IOSurfaceRef, or AHardwareBuffer */
        SDL_FalconExternalTexturePlane planes[SDL_FALCON_EXTERNAL_TEXTURE_MAX_PLANES];
    } native;
} SDL_FalconExternalTexture;

/* Records a native-to-SDL_GPU texture copy in command_buffer. The source
 * handle/FDs are borrowed and must remain valid until the submitted copy has
 * completed. Unsupported platform imports fail; there is no CPU fallback. */
extern SDL_FALCON_GPU_API bool SDLCALL SDL_FalconCopyExternalTexture(
    SDL_GPUDevice *device,
    SDL_GPUCommandBuffer *command_buffer,
    const SDL_FalconExternalTexture *source,
    SDL_GPUTexture *destination);

#ifdef __cplusplus
}
#endif

#endif /* SDL_falcon_gpu_h_ */
