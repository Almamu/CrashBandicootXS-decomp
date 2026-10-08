#ifndef GUARD_CONSTANTS_CHUNK_TOKENS_H
#define GUARD_CONSTANTS_CHUNK_TOKENS_H

/*
 * The run kinds of a layer or collision chunk's token stream
 * (DecodeCollisionChunk, bg_layer_base.cpp, and its twin DecodeLayerChunk,
 * cutscene_player.cpp; docs/rom_map.md). Each token halfword's low byte is
 * the run length, and its top bits pick the run: a token with neither bit
 * set copies the next `n` halfwords as they are.
 */
#define CHUNK_TOKEN_FILL 0x8000  /* one halfword, written `n` times */
#define CHUNK_TOKEN_DELTA 0x4000 /* a start halfword, then signed byte deltas, two per halfword */

#endif /* GUARD_CONSTANTS_CHUNK_TOKENS_H */
