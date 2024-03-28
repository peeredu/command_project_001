<template>
    <item-card v-bind:item="item"/>
</template>

<script>
import axios from 'axios'

import ItemCard from '@/components/ItemCard'

export default {
    components: {
        ItemCard,
    },
    data () {
        return {
            item: {
                name: '',
                price: 0,
                quantity: 0,
                in_stock: false,
            },
        }
    },
}


</script>

<style>

</style>