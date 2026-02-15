# Open Plexus Framework (v0.1)

This document defines a first-pass framework for building Open Plexus as a mod-friendly expansion layer on top of base SM64.

## Goals

- Keep early game behavior close to vanilla SM64.
- Apply rare, persistent "drift" changes during early progression.
- Unlock major content at 70 stars.
- Let contributors add custom areas/events without editing core game logic every time.

## Core Concepts

### 1) Save-seeded world state

Each save file owns a deterministic seed and a set of persistent flags.

- `save_seed`: generated once when the save is created.
- `event_flags`: permanent unlocks and world changes.
- `mount_table`: selected active mount points for doors/warps.
- `painting_table`: selected painting visuals and destinations.

Because generation is seed-based, players can share "world codes" while still keeping run-to-run variation across save files.

### 2) Progression phases

- **Phase 0 (0-69 stars):** mostly vanilla, rare low-impact drift events.
- **Phase 1 (70+ stars):** major unlocks, expanded door mount points, high-impact area/event pool.

### 3) Data-driven content packs

Contributors define content with manifests instead of hard-coding behavior in many game files.

Each pack can provide:

- Doors / mount points
- Event triggers and effects
- Painting mappings (visual source separate from destination level)
- Weighted random rules by phase

## Framework Layout

This repo now includes a starter folder:

- `enhancements/plexus_framework/README.md`
- `enhancements/plexus_framework/plexus_schema.json`
- `enhancements/plexus_framework/examples/basic_pack.json`

Use these as authoring references for new packs.

## Runtime Pipeline (proposed)

1. On save load, initialize or read the save seed + persistent framework state.
2. Load and validate content packs.
3. Filter entries by progression phase and conditions.
4. Run weighted deterministic selection (seeded RNG).
5. Write selected results into save-backed tables.
6. Apply results during level setup and castle object spawn.

## Initial Scope

This is a framework definition layer only.

- It does not replace existing level scripts yet.
- It defines manifest shape and expected runtime behavior so implementation can be incremental.

## Next Implementation Steps

1. Add a parser/validator for pack manifests in `src/pc` tooling or compile-time scripts.
2. Add a save extension block for framework state.
3. Hook castle setup logic to mount-point table resolution.
4. Hook painting logic so painting visuals and destinations are decoupled.
5. Add dev/debug menu for viewing active events and rerolling world state.
