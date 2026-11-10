/* Falcon-specific SDL_GPU bridge for accelerated CEF UI frames. */

#define SDL_FALCON_GPU_BUILD 1

#include "SDL_internal.h"
#include "SDL_sysgpu.h"

SDL_FALCON_GPU_API bool SDLCALL SDL_FalconCopyExternalTexture(
    SDL_GPUDevice *device,
    SDL_GPUCommandBuffer *commandBuffer,
    const SDL_FalconExternalTexture *source,
    SDL_GPUTexture *destination)
{
    if (device == NULL) {
        SDL_SetError("Invalid GPU device");
        return false;
    }
    if (source == NULL || device->CopyExternalTexture == NULL) {
        SDL_SetError("The selected SDL_GPU backend cannot import this external texture");
        return false;
    }
    return device->CopyExternalTexture(
        device->driverData,
        commandBuffer,
        source,
        destination);
}
