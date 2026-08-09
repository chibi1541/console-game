# CLAUDE.md

## Purpose
This project uses Claude to generate console-game character animations made of
**box-drawing / geometric Unicode characters** (e.g. `┃ │ ─ ╱ ╲ ╹ ╺ ▸ ◂` and
similar box-drawing / shape characters). Do not use plain ASCII stick-figure
characters (`O /|\` etc.) or block-shading characters (`█ ▓ ▒ ░`) unless the
user explicitly asks for that style in a given request.

## Scope
- Only perform the character-animation generation task described below.
- Do not perform any other task (refactoring unrelated code, adding features,
  writing documentation, installing packages, etc.) unless the user explicitly
  requests it in the same message.
- If a request is ambiguous, ask a short clarifying question instead of
  guessing and doing extra work.

## Output Format
- Each animation is a sequence of **frames**. Generate **2–3 frames per
  motion** (no more, no fewer, unless the user specifies otherwise).
- Each individual frame must be saved as its **own separate `.txt` file**.
- File naming convention:
  `{character_name}_{motion}_frame{N}.txt`
  Example: `warrior_walk_frame1.txt`, `warrior_walk_frame2.txt`
- All frame files must be UTF-8 encoded (required for box-drawing characters
  to render correctly).

## File Location
- All generated frame files must be written under `Assets/`, inside a folder
  for the **object type** and then a subfolder for the **animation (motion)
  name**:
  `Assets/{object_type}/{motion}/{character_name}_{motion}_frame{N}.txt`
  Example: `Assets/Warrior/Walk/warrior_walk_frame1.txt`
- Create the `{object_type}` and `{motion}` folders if they do not exist.
- Never write frame files directly into `Assets/` root or into any other
  directory.

## Sizing Rules
- **Width consistency**: within the same animation (same character + same
  motion), every frame must have the **same line width**. Pad shorter lines
  with trailing spaces so all lines in all frames of that animation match the
  widest line.
- **Height consistency**: within the same animation, all frames should have
  the same number of lines (rows). Pad with blank lines if needed.
- After generating each animation, report its final size to the user as:
  `Width: {N} characters, Height: {N} lines`
  (report this once per animation, not once per frame, since all frames in
  the same animation share the same size).

## Required Default Motions
When asked to create a new character, generate all five of the following
motions by default (unless the user asks for only a subset):
1. **Idle** (대기)
2. **Move** (이동 / walk)
3. **Hit** (피격)
4. **Attack** (공격)
5. **Down** (다운)

## Style Consistency
- All frames of all motions for the same character should share a
  recognizable silhouette (same general head/body shape and character set)
  so the character reads as the same entity across motions.
- Keep designs minimal in detail — the goal is a small, clean silhouette,
  not a highly detailed illustration.

## Character Templates
These are established character archetypes with their own distinct character
sets. When the user asks for one of these by name, reuse its character set
for consistency; when creating a new archetype, follow the same pattern
(pick a distinct subset of characters so each archetype has its own visual
identity).

| Archetype | Character set (typical) | Notes |
|---|---|---|
| Warrior (전사) | `█ ▓ ▐ ▌ ╱ ╲ ┃ ╹` | blocky, heavy silhouette |
| Mage (마법사) | `◆ ◇ ▓ ╱ ╲ ╹` | diamond hood/robe, soft outline |
| Archer (궁수) | `○ █ ▕ ▏ ╱ ╲ ┃` | round head, thin body, light build |
| Ninja (닌자) | `◢ ▲ ◣ ▐ ▓ ▌ ╱ ╲ ┃` | triangular hood, sharp angular lines |
| Slime (슬라임) | `● ○ ◐ ◑` | round-only, squash-and-stretch body |
| Robot (로봇) | `■ □ ▪ ▫ ┌ ┐ └ ┘ ┃ ╻ ─` | square edges, mechanical joints |
| Ghost (유령) | `◆ ◇ ▓ ≈ ~` | diamond head, wavy/floating lower body |
| Beast (소형 몬스터) | `▲ ▼ ◀ ▶ ▓` | triangle-only, small spiky silhouette |

## Language
- Always reply in the **same language the user used in their message**.
  If the user writes in Korean, reply in Korean; if in English, reply in
  English; and so on.
