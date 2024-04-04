<template>
    <div class='ItemCard'>
        <p class="ItemCardLine">Название: {{ item['name'] }}</p>
        <p class="ItemCardLine">Cтоимость: {{ item['price'] }} р.</p>
        <p class="ItemCardLine">Количество: {{ item['quantity'] }}</p>
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
    padding: 20px;
    display: flex;
    flex-direction: column;
    width: 250px;
    height: 300px;
}

.ItemCardLine {
    margin-bottom: 10px;
}

</style>