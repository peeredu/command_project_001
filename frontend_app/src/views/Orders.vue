<template>

</template>

<script>
import axios from 'axios'

export default {
    components: {

    },
    data () {
        return {
            orders: [],
        }
    },
    methods: {
        async getOrders() {
            axios
                .get('/orders')
                .then((response) => {
                    this.orders = response.data;
                })
        }
    },
}


</script>

<style>

</style>