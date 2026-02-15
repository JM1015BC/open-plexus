#include "plexus_framework.h"

#include "course_table.h"
#include "engine/behavior_script.h"
#include "save_file.h"

#define PLEXUS_SHIFT_MIN_TIMER 1800
#define PLEXUS_SHIFT_TIMER_VARIANCE 3600

struct PlexusRuntime {
    s16 nextShiftTimer;
    u8 phase;
    u8 initialized;
};

static struct PlexusRuntime sPlexusRuntime;
static struct PlexusDoorMount sDoorMounts[PLEXUS_MAX_DOOR_MOUNTS];
static struct PlexusPaintingRoute sPaintingRoutes[PLEXUS_MAX_PAINTING_ROUTES];

static u16 sDoorMountCount;
static u16 sPaintingRouteCount;

static u16 plexus_get_random_timer(void) {
    return PLEXUS_SHIFT_MIN_TIMER + (random_u16() % PLEXUS_SHIFT_TIMER_VARIANCE);
}

u32 plexus_framework_get_world_shift_flags(void) {
    return save_file_get_flags() & SAVE_FLAG_PLEXUS_WORLD_SHIFT_MASK;
}

s32 plexus_framework_is_network_open(void) {
    return (save_file_get_flags() & SAVE_FLAG_PLEXUS_NETWORK_OPEN) != 0;
}

static void plexus_update_phase(s32 starCount) {
    if (plexus_framework_is_network_open() || starCount >= PLEXUS_NETWORK_STAR_REQUIREMENT) {
        if (!plexus_framework_is_network_open()) {
            save_file_set_flags(SAVE_FLAG_PLEXUS_NETWORK_OPEN);
        }

        sPlexusRuntime.phase = PLEXUS_PHASE_NETWORK_OPEN;
        return;
    }

    if (starCount >= 15) {
        sPlexusRuntime.phase = PLEXUS_PHASE_DRIFT;
    } else {
        sPlexusRuntime.phase = PLEXUS_PHASE_VANILLA;
    }
}

s32 plexus_framework_get_phase(void) {
    return sPlexusRuntime.phase;
}

static void plexus_commit_random_world_shift(void) {
    u32 availableShifts[3];
    u32 existingShifts = plexus_framework_get_world_shift_flags();
    s32 availableCount = 0;

    if ((existingShifts & SAVE_FLAG_PLEXUS_SHIFT_CORRIDOR) == 0) {
        availableShifts[availableCount++] = SAVE_FLAG_PLEXUS_SHIFT_CORRIDOR;
    }

    if ((existingShifts & SAVE_FLAG_PLEXUS_SHIFT_PAINTING_FRAME) == 0) {
        availableShifts[availableCount++] = SAVE_FLAG_PLEXUS_SHIFT_PAINTING_FRAME;
    }

    if ((existingShifts & SAVE_FLAG_PLEXUS_SHIFT_CASTLE_PROP) == 0) {
        availableShifts[availableCount++] = SAVE_FLAG_PLEXUS_SHIFT_CASTLE_PROP;
    }

    if (availableCount > 0) {
        save_file_set_flags(availableShifts[random_u16() % availableCount]);
    }
}

void plexus_framework_on_level_init(void) {
    s32 starCount;

    if (gCurrSaveFileNum == 0 || !save_file_exists(gCurrSaveFileNum - 1)) {
        sPlexusRuntime.phase = PLEXUS_PHASE_VANILLA;
        sPlexusRuntime.nextShiftTimer = 0;
        sPlexusRuntime.initialized = TRUE;
        return;
    }

    starCount = save_file_get_total_star_count(gCurrSaveFileNum - 1, COURSE_MIN - 1, COURSE_MAX - 1);
    plexus_update_phase(starCount);

    if (sPlexusRuntime.nextShiftTimer <= 0) {
        sPlexusRuntime.nextShiftTimer = plexus_get_random_timer();
    }

    sPlexusRuntime.initialized = TRUE;
}

void plexus_framework_update(void) {
    s32 starCount;

    if (!sPlexusRuntime.initialized) {
        plexus_framework_on_level_init();
    }

    if (gCurrSaveFileNum == 0 || !save_file_exists(gCurrSaveFileNum - 1)) {
        return;
    }

    starCount = save_file_get_total_star_count(gCurrSaveFileNum - 1, COURSE_MIN - 1, COURSE_MAX - 1);
    plexus_update_phase(starCount);

    if (sPlexusRuntime.phase == PLEXUS_PHASE_VANILLA || sPlexusRuntime.phase == PLEXUS_PHASE_NETWORK_OPEN) {
        return;
    }

    if (--sPlexusRuntime.nextShiftTimer <= 0) {
        plexus_commit_random_world_shift();
        sPlexusRuntime.nextShiftTimer = plexus_get_random_timer();
    }
}

s32 plexus_framework_register_door_mount(const struct PlexusDoorMount *mount) {
    if (mount == NULL || sDoorMountCount >= PLEXUS_MAX_DOOR_MOUNTS) {
        return FALSE;
    }

    sDoorMounts[sDoorMountCount] = *mount;
    sDoorMounts[sDoorMountCount].isActive = TRUE;
    sDoorMountCount++;
    return TRUE;
}

void plexus_framework_clear_door_mounts(void) {
    sDoorMountCount = 0;
}

s32 plexus_framework_get_door_mount_count(void) {
    return sDoorMountCount;
}

s32 plexus_framework_register_painting_route(const struct PlexusPaintingRoute *route) {
    if (route == NULL || sPaintingRouteCount >= PLEXUS_MAX_PAINTING_ROUTES) {
        return FALSE;
    }

    sPaintingRoutes[sPaintingRouteCount] = *route;
    sPaintingRoutes[sPaintingRouteCount].isActive = TRUE;
    sPaintingRouteCount++;
    return TRUE;
}

void plexus_framework_clear_painting_routes(void) {
    sPaintingRouteCount = 0;
}

s32 plexus_framework_get_painting_route_count(void) {
    return sPaintingRouteCount;
}
