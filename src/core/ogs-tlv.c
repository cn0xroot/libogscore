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

#include "ogs-core.h"

#undef OGS_LOG_DOMAIN
#define OGS_LOG_DOMAIN __ogs_tlv_domain

static OGS_POOL(pool, ogs_tlv_t);

/* ogs_tlv_t common functions */
ogs_tlv_t* ogs_tlv_get(void)
{
    ogs_tlv_t *tlv = NULL;

    /* get tlv node from node pool */
    ogs_pool_alloc(&pool, &tlv);

    /* check for error */
    ogs_assert(tlv);

    /* intialize tlv node */
    memset((char*)tlv, 0x00, sizeof(ogs_tlv_t));
    return tlv;
}

void ogs_tlv_free(ogs_tlv_t *p_tlv)
{
    /* free tlv node to the node pool */
    ogs_pool_free(&pool, p_tlv);
}

void ogs_tlv_init(void)
{
    ogs_pool_init(&pool, ogs_core()->tlv.pool);
}

void ogs_tlv_final(void)
{
    ogs_pool_final(&pool);
}

uint32_t ogs_tlv_pool_avail(void)
{
    return ogs_pool_avail(&pool);
}

void ogs_tlv_free_all(ogs_tlv_t *root_tlv)
{
    /* free all tlv node to the node pool */
    ogs_tlv_t *p_tlv = root_tlv;
    ogs_tlv_t *next = NULL;
    while(p_tlv) {
        if(p_tlv->embedded != NULL) {
            ogs_tlv_free_all(p_tlv->embedded);
        }
        next = p_tlv->next;
        ogs_tlv_free(p_tlv);
        p_tlv = next;
    }
    return;
}

uint8_t ogs_tlv_value_8(ogs_tlv_t *tlv)
{
    return (*((uint8_t*)(tlv->value)));
}

uint16_t ogs_tlv_value_16(ogs_tlv_t *tlv)
{
    uint16_t u_16;
    uint8_t *v = tlv->value;

    u_16 = ((v[0] <<  8) & 0xff00) |
           ((v[1]      ) & 0x00ff);

    return u_16;
}

uint32_t ogs_tlv_value_32(ogs_tlv_t *tlv)
{
    uint32_t u_32;
    uint8_t *v = tlv->value;

    u_32 = ((v[0] << 24) & 0xff000000) |
           ((v[1] << 16) & 0x00ff0000) |
           ((v[2] <<  8) & 0x0000ff00) |
           ((v[3]      ) & 0x000000ff);

    return u_32;
}

uint32_t ogs_tlv_calc_length(ogs_tlv_t *p_tlv, uint8_t mode)
{
    ogs_tlv_t *tmp_tlv = p_tlv;
    uint32_t length = 0;

    while(tmp_tlv) {
        /* this is length for type field */
        switch(mode) {
        case OGS_TLV_MODE_T1_L1:
            length += 2;
            break;
        case OGS_TLV_MODE_T1_L2:
            length += 3;
            break;
        case OGS_TLV_MODE_T1_L2_I1:
        case OGS_TLV_MODE_T2_L2:
            length += 4;
            break;
        default:
            ogs_assert_if_reached();
            break;
        }

        /* this is length for type field */
        if(tmp_tlv->embedded != NULL) {
            tmp_tlv->length = ogs_tlv_calc_length(tmp_tlv->embedded, mode);
        }

        /* this is length for value field */
        length += tmp_tlv->length;

        tmp_tlv = tmp_tlv->next;
    }
    return length;
}

uint32_t ogs_tlv_calc_count(ogs_tlv_t *p_tlv)
{
    ogs_tlv_t *tmp_tlv = p_tlv;
    uint32_t count = 0;

    while(tmp_tlv) {
        if(tmp_tlv->embedded != NULL) {
            count += ogs_tlv_calc_count(tmp_tlv->embedded);
        } else {
            count++;
        }
        tmp_tlv = tmp_tlv->next;
    }
    return count;
}

static uint8_t *tlv_put_type(uint32_t type, uint8_t *pos, uint8_t mode)
{    
    switch(mode) {
    case OGS_TLV_MODE_T1_L1:
    case OGS_TLV_MODE_T1_L2:
    case OGS_TLV_MODE_T1_L2_I1:
        *(pos++) = type & 0xFF;
        break;
    case OGS_TLV_MODE_T2_L2:
        *(pos++) = (type >> 8) & 0xFF;
        *(pos++) = type & 0xFF;
        break;
    default:
        ogs_assert_if_reached();
        break;
    }
    return pos;
}

static uint8_t *tlv_put_length(uint32_t length, uint8_t *pos, uint8_t mode)
{
    switch(mode) {
    case OGS_TLV_MODE_T1_L1:
        *(pos++) = length & 0xFF;
        break;
    case OGS_TLV_MODE_T1_L2:
    case OGS_TLV_MODE_T1_L2_I1:
    case OGS_TLV_MODE_T2_L2:
        *(pos++) = (length >> 8) & 0xFF;
        *(pos++) = length & 0xFF;
        break;
    default:
        ogs_assert_if_reached();
        break;
    }

    return pos;
}

static uint8_t *tlv_put_instance(uint8_t instance, uint8_t *pos, uint8_t mode)
{
    switch(mode) {
        case OGS_TLV_MODE_T1_L2_I1:
            *(pos++) = instance & 0xFF;
            break;
        default:
            break;
    }

    return pos;
}

static uint8_t *tlv_get_element(ogs_tlv_t *p_tlv, uint8_t *tlvBlock, uint8_t mode)
{
    uint8_t *pos = tlvBlock;

    switch(mode) {
    case OGS_TLV_MODE_T1_L1:
        p_tlv->type = *(pos++);
        p_tlv->length = *(pos++);
        break;
    case OGS_TLV_MODE_T1_L2:
        p_tlv->type = *(pos++);
        p_tlv->length = *(pos++) << 8;
        p_tlv->length += *(pos++);
        break;
    case OGS_TLV_MODE_T1_L2_I1:
        p_tlv->type = *(pos++);
        p_tlv->length = *(pos++) << 8;
        p_tlv->length += *(pos++);
        p_tlv->instance = *(pos++);
        break;
    case OGS_TLV_MODE_T2_L2:
        p_tlv->type = *(pos++) << 8;
        p_tlv->type += *(pos++);
        p_tlv->length = *(pos++) << 8;
        p_tlv->length += *(pos++);
        break;
    default:
        ogs_assert_if_reached();
        break;
    }

    p_tlv->value = pos;

    return (pos + ogs_tlv_length(p_tlv));
}

static void tlv_alloc_buff_to_tlv(
        ogs_tlv_t* head_tlv, uint8_t* buff, uint32_t buff_len)
{
    head_tlv->buff_allocated = true;
    head_tlv->buff_len = buff_len;
    head_tlv->buff_ptr = buff;
    head_tlv->buff = buff;
}

ogs_tlv_t *ogs_tlv_find_root(ogs_tlv_t* p_tlv)
{
    ogs_tlv_t *head_tlv = p_tlv->head;
    ogs_tlv_t *parentTlv;

    parentTlv = head_tlv->parent;
    while(parentTlv) {
        head_tlv = parentTlv->head;
        parentTlv = head_tlv->parent;
    }

    return head_tlv;
}

ogs_tlv_t *ogs_tlv_add(ogs_tlv_t *head_tlv, 
    uint32_t type, uint32_t length, uint8_t instance, uint8_t *value)
{
    ogs_tlv_t* curr_tlv = head_tlv;
    ogs_tlv_t* new_tlv = NULL;

    new_tlv = ogs_tlv_get();
    ogs_assert(new_tlv);
    if(length != 0)
        ogs_assert(value);

    new_tlv->type = type;
    new_tlv->length = length;
    new_tlv->instance = instance;
    new_tlv->value = value;

    if (head_tlv != NULL && head_tlv->buff_allocated == true) {
        ogs_assert((head_tlv->buff_ptr - head_tlv->buff + length) < 
                head_tlv->buff_len);

        memcpy(head_tlv->buff_ptr, value, length);
        new_tlv->value = head_tlv->buff_ptr;
        head_tlv->buff_ptr += length;
    }

    if(curr_tlv == NULL) {
        new_tlv->head = new_tlv;
        new_tlv->tail = new_tlv;
    } else {
        head_tlv = head_tlv->head; /* in case head_tlv is not head */
        new_tlv->head = head_tlv;
        head_tlv->tail->next = new_tlv;
        head_tlv->tail = new_tlv;
    }
    return new_tlv;
}

ogs_tlv_t *ogs_tlv_copy(uint8_t *buff, uint32_t buff_len,
    uint32_t type, uint32_t length, uint8_t instance, uint8_t *value)
{
    ogs_tlv_t* new_tlv = NULL;

    new_tlv = ogs_tlv_get();
    ogs_assert(new_tlv);

    new_tlv->type = type;
    new_tlv->length = length;
    new_tlv->instance = instance;
    new_tlv->value = value;
    new_tlv->head = new_tlv->tail = new_tlv;

    tlv_alloc_buff_to_tlv(new_tlv, buff, buff_len);

    memcpy(new_tlv->buff_ptr, value, length);
    new_tlv->value = new_tlv->buff_ptr;
    new_tlv->buff_ptr += length;

    return new_tlv;
}

ogs_tlv_t *ogs_tlv_embed(ogs_tlv_t *parent_tlv, 
    uint32_t type, uint32_t length, uint8_t instance, uint8_t *value)
{
    ogs_tlv_t* new_tlv = NULL, *root_tlv = NULL;

    ogs_assert(parent_tlv);

    new_tlv = ogs_tlv_get();
    ogs_assert(new_tlv);

    new_tlv->type = type;
    new_tlv->length = length;
    new_tlv->instance = instance;
    new_tlv->value = value;

    root_tlv = ogs_tlv_find_root(parent_tlv);

    if(root_tlv->buff_allocated == true) {
        ogs_assert((root_tlv->buff_ptr - root_tlv->buff + length) < 
                root_tlv->buff_len);

        memcpy(root_tlv->buff_ptr, value, length);
        new_tlv->value = root_tlv->buff_ptr;
        root_tlv->buff_ptr += length;
    }

    if(parent_tlv->embedded == NULL) {
        parent_tlv->embedded = new_tlv->head = new_tlv->tail = new_tlv;
        new_tlv->parent = parent_tlv;
    } else {
        new_tlv->head = parent_tlv->embedded;
        parent_tlv->embedded->tail->next = new_tlv;
        parent_tlv->embedded->tail = new_tlv;
    }

    return new_tlv;
}

uint32_t ogs_tlv_render(ogs_tlv_t *root_tlv, 
    uint8_t *blk, uint32_t length, uint8_t mode)
{
    ogs_tlv_t* curr_tlv = root_tlv;
    uint8_t* pos = blk;
    uint32_t embedded_len = 0;

    while(curr_tlv) {
        pos = tlv_put_type(curr_tlv->type, pos, mode);

        if(curr_tlv->embedded == NULL) {
            pos = tlv_put_length(curr_tlv->length, pos, mode);
            pos = tlv_put_instance(curr_tlv->instance, pos, mode);

            if ((pos - blk) + ogs_tlv_length(curr_tlv) > length)
                ogs_assert_if_reached();

            memcpy((char*)pos, (char*)curr_tlv->value, curr_tlv->length);
            pos += curr_tlv->length;
        } else {
            embedded_len = ogs_tlv_calc_length(curr_tlv->embedded, mode);
            pos = tlv_put_length(embedded_len, pos, mode);
            pos = tlv_put_instance(curr_tlv->instance, pos, mode);
            ogs_tlv_render(curr_tlv->embedded,
                pos, length - (uint32_t)(pos-blk), mode);
            pos += embedded_len;
        }
        curr_tlv = curr_tlv->next;
    }

    return (pos - blk);
}

/* ogs_tlv_t parsing functions */
ogs_tlv_t *ogs_tlv_parse_block(uint32_t length, uint8_t *blk, uint8_t mode)
{
    uint8_t* pos = blk;

    ogs_tlv_t* root_tlv = NULL;
    ogs_tlv_t* prev_tlv = NULL;
    ogs_tlv_t* curr_tlv = NULL;

    root_tlv = curr_tlv = ogs_tlv_get();

    ogs_assert(curr_tlv);

    pos = tlv_get_element(curr_tlv, pos, mode);

    ogs_assert(pos);

    while(pos - blk < length) {
        prev_tlv = curr_tlv;

        curr_tlv = ogs_tlv_get();
        ogs_assert(curr_tlv);
        prev_tlv->next = curr_tlv;

        pos = tlv_get_element(curr_tlv, pos, mode);
        ogs_assert(pos);
    }

    ogs_assert(length == (pos - blk));

    return root_tlv;
}

ogs_tlv_t *ogs_tlv_parse_embedded_block(ogs_tlv_t* p_tlv, uint8_t mode)
{
    p_tlv->embedded = ogs_tlv_parse_block(p_tlv->length, p_tlv->value, mode);

    return p_tlv->embedded;
}

/* tlv operation-related function */
ogs_tlv_t *ogs_tlv_find(ogs_tlv_t* p_tlv, uint32_t type)
{
    ogs_tlv_t *tmp_tlv = p_tlv, *embed_tlv = NULL;
    while(tmp_tlv) {
        if(tmp_tlv->type == type) {
            return tmp_tlv;
        }

        if(tmp_tlv->embedded != NULL) {
            embed_tlv = ogs_tlv_find(tmp_tlv->embedded, type);
            if(embed_tlv != NULL) {
                return embed_tlv;
            }
        }
        tmp_tlv = tmp_tlv->next;
    }

    /* tlv for the designated type doesn't exist */
    return NULL;
}
