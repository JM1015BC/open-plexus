#ifndef PLEXUS_FRAMEWORK_H
#define PLEXUS_FRAMEWORK_H

#include <PR/ultratypes.h>

#define PLEXUS_NETWORK_STAR_REQUIREMENT 70
#define PLEXUS_MAX_DOOR_MOUNTS 16
#define PLEXUS_MAX_PAINTING_ROUTES 32

enum PlexusPhase {
    PLEXUS_PHASE_VANILLA,
    PLEXUS_PHASE_DRIFT,
    PLEXUS_PHASE_NETWORK_OPEN
};

struct PlexusDoorMount {
    u8 sourceLevel;
    u8 sourceArea;
    u8 sourceWarpNode;
    u8 destinationLevel;
    u8 destinationArea;
    u8 destinationWarpNode;
    u8 isActive;
};

struct PlexusPaintingRoute {
    u8 paintingId;
    u8 destinationLevel;
    u8 destinationArea;
    u8 destinationWarpNode;
    u8 weight;
    u8 isActive;
};

void plexus_framework_on_level_init(void);
void plexus_framework_update(void);

u32 plexus_framework_get_world_shift_flags(void);
s32 plexus_framework_get_phase(void);
s32 plexus_framework_is_network_open(void);

s32 plexus_framework_register_door_mount(const struct PlexusDoorMount *mount);
void plexus_framework_clear_door_mounts(void);
s32 plexus_framework_get_door_mount_count(void);

s32 plexus_framework_register_painting_route(const struct PlexusPaintingRoute *route);
void plexus_framework_clear_painting_routes(void);
s32 plexus_framework_get_painting_route_count(void);

#endif // PLEXUS_FRAMEWORK_H
