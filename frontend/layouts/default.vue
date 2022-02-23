<template>
  <div class="min-w-screen min-h-screen bg-gradient-to-br from-indigo-500 via-purple-500 to-fuchsia-400">
    <div class="w-full bg-transparent/20 h-16 border-b">
      <div class="w-full h-full flex flex-row justify-center items-center space-x-20">
        <div class="menu-item" @click="$router.push('/database')">
          <p>База</p>
        </div>
        <div class="menu-item" @click="$router.push('/benchmark')">
          <p>Тестирование</p>
        </div>
      </div>
    </div>
    <div class="mt-8">
      <nuxt />
    </div>
  </div>
</template>

<style scoped>
.menu-item {
  @apply p-2;
  @apply cursor-pointer;
  @apply text-white font-bold text-2xl;
  @apply transition-all duration-200;
  @apply rounded-full;
  @apply hover:text-black hover:bg-white;
}

</style>
