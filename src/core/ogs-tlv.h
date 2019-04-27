/*
 * Copyright (C) 2019 by Sukchan Lee <acetcom@gmail.com>
 *
 * This file is part of Open5GS.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef OGS_TLV_H
#define OGS_TLV_H

#if !defined(OGS_CORE_INSIDE)
#error "Only <ogs-core.h> can be included directly."
#endif

#define OGS_TLV_MODE_T1_L1              1
#define OGS_TLV_MODE_T1_L2              2
#define OGS_TLV_MODE_T1_L2_I1           3
#define OGS_TLV_MODE_T2_L2              4

/* ogs_tlv_t struncture */

typedef struct ogs_tlv_s
{
    /* for tlv management */
    struct ogs_tlv_s *head;
    struct ogs_tlv_s *tail;  /* this is used only for head ogs_tlv_t */
    struct ogs_tlv_s *next;

    struct ogs_tlv_s *parent;
    struct ogs_tlv_s *embedded;

    /* tlv basic element */
    uint32_t type;
    uint32_t length;
    uint8_t instance;
    void* value;

    /* can be needed in encoding ogs_tlv_t*/
    bool buff_allocated;
    uint32_t buff_len;
    uint8_t *buff_ptr;
    uint8_t *buff;
} ogs_tlv_t;

#define ogs_tlv_type(pTlv) pTlv->type
#define ogs_tlv_length(pTlv) pTlv->length
#define ogs_tlv_instance(pTlv) pTlv->instance
#define ogs_tlv_value(pTlv) pTlv->value

/* ogs_tlv_t pool related functions */
ogs_tlv_t* ogs_tlv_get(void);
void ogs_tlv_free(ogs_tlv_t *pTlv);
void ogs_tlv_free_all(ogs_tlv_t *rootTlv);

void ogs_tlv_init(void);
void ogs_tlv_final(void);

uint32_t ogs_tlv_pool_avail(void);

/* ogs_tlv_t encoding functions */
ogs_tlv_t* ogs_tlv_add(ogs_tlv_t *headTlv, 
    uint32_t type, uint32_t length, uint8_t instance, uint8_t *value);
ogs_tlv_t* ogs_tlv_copy(uint8_t *buff, uint32_t buff_len,
    uint32_t type, uint32_t length, uint8_t instance, uint8_t *value);
ogs_tlv_t* ogs_tlv_embed(ogs_tlv_t *parent_tlv, 
    uint32_t type, uint32_t length, uint8_t instance, uint8_t *value);

uint32_t ogs_tlv_render(
        ogs_tlv_t *rootTlv, uint8_t *blk, uint32_t length, uint8_t mode);

/* ogs_tlv_t parsing functions */
ogs_tlv_t* ogs_tlv_parse_block(
        uint32_t length, uint8_t *blk, uint8_t mode);
ogs_tlv_t* ogs_tlv_parse_embedded_block(ogs_tlv_t* pTlv, uint8_t mode);

/* tlv operation-related function */
ogs_tlv_t* ogs_tlv_find(ogs_tlv_t* pTlv, uint32_t type);
ogs_tlv_t* ogs_tlv_find_root(ogs_tlv_t* pTlv);
uint32_t ogs_tlv_calc_length(ogs_tlv_t *p_tlv, uint8_t mode);
uint32_t ogs_tlv_calc_count(ogs_tlv_t *p_tlv);
uint8_t ogs_tlv_value_8(ogs_tlv_t *tlv);
uint16_t ogs_tlv_value_16(ogs_tlv_t *tlv);
uint32_t ogs_tlv_value_32(ogs_tlv_t *tlv);

#ifdef __cplusplus
}
#endif

#endif /* OGS_OGS_TLV_H */
