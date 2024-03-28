<template>
    <div class='ItemCard'>
        <p>{{ item.name }}</p>
        <p>{{ item.price }}</p>
        <p>{{ item.quantity }}</p>
        <p>{{ item.in_stock }}</p>
    </div>
</template>

<script>
    export default {
        props: {
            item: {
                required: true,
            } 
        }
    }
</script>

<style scoped>

.ItemCard {
    display: flex;
    flex-direction: column;
    width: 150px;
    height: 200px;
}

</style>