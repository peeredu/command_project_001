<template>

</template>

<script>
import axios from 'axios'

export default {
    components: {

    },
    data () {
        return {
            order: {
                items_ids: [],
                price: 0,
                date: null,
            },
        }
    },
    methods: {
        async getSingleOrder() {
            const orderId = this.$route.params.id
            axios
                .get('/orders/${orderId}')
                .then((response) => {
                    this.item = response.data;
                })
        }
    },
}


</script>

<style>

</style>