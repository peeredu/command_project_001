<template>
  <button type="submit" class="FormButton">{{ service.text }}</button>
</template>

<script>
export default {
  props: {
    service: {},
  },
};
</script>

<style scoped>
.FormButton {
  display: inline-block;
  border: 1px solid rgb(32, 73, 178);
  margin-bottom: 15px;
  width: 150px;
  height: 50px;
  background-color: rgb(207, 216, 255);
  cursor: pointer;
  line-height: 50px;
  text-align: center;
  border-radius: 8px;
}
</style>
