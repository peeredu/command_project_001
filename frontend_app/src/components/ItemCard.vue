<template>
    <div class='ItemCard'>
        <p v-for="prop in Object.keys(item)">{{ item[prop] }}</p>
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