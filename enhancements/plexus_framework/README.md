# Plexus Framework Pack Format

This folder contains a starter content-pack framework for Open Plexus.

## Why this exists

The project goal is to let contributors add world changes without deeply editing core engine files each time.

This format focuses on data-first definitions for:

- progression-gated changes,
- random but deterministic world generation,
- permanent changes per save file.

## Files

- `plexus_schema.json`: JSON schema for validating pack files.
- `examples/basic_pack.json`: minimal example pack.

## Pack loading model (design target)

1. Read pack files.
2. Validate against `plexus_schema.json`.
3. Merge packs by `id` and `priority`.
4. For each category (mounts, events, paintings), pick active entries by:
   - progression phase,
   - conditions,
   - weighted deterministic RNG seeded from save data.
5. Persist selected outcomes to save data.

## Conventions

- `id` values should be globally unique and namespaced (`author.feature_name`).
- `weight` defaults to 1 if omitted.
- `phase` can be:
  - `early` (0-69 stars)
  - `late` (70+ stars)
  - `any`
- `once_per_save: true` means selected events should never reroll unless save is reset.

## Notes

This is intentionally a lightweight framework scaffold, not a finalized runtime implementation.
