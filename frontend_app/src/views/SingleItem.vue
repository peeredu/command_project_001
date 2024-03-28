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
    methods: {
        async getSingleItem() {
            const itemId = this.$route.params.id
            axios
                .get('/items/${itemId}')
                .then((response) => {
                    this.item = response.data;
                })
        }
    },
}


</script>

<style>

</style>