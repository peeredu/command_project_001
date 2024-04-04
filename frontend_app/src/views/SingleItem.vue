<template>
    <item-filter></item-filter>
    <item-card v-bind:item="item"/>
</template>

<script>
import axios from 'axios'

import ItemCard from '@/components/ItemCard'
import ItemFilter from '@/components/ItemFilter'

export default {
    components: {
        ItemCard,
        ItemFilter,
    },
    data () {
        return {
            item: {
                id: 0,
                name: 'empty',
                price: 0,
                quantity: 0,
                active: false,
            },
        }
    },
    methods: {
        async getItem() {
            axios.get("http://localhost:2000/items/{id}").then((response) => {
                this.item = response.data['item']
            })
        }
    }
}


</script>

<style>

</style>