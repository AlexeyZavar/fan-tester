<template>
  <div class="flex justify-center">
    <div class="grid grid-cols-2 gap-8">
      <div v-for="item in items" :key="item.id" class="card">
        <p>{{ item.name }}</p>
        <hr>
        <p class="mt-4">
          ⏰ Тестировалось: {{ new Date(item.performed_on * 1000).toLocaleString() }}
        </p>
        <button class="btn" @click="$router.push(`/item?id=${item.id}`)">
          Подробнее →
        </button>
      </div>
    </div>
  </div>
</template>

<script lang="ts">
import Vue from 'vue'

export interface DatabaseItem {
  id: string
  name: string
  soft_start: boolean
  performed_on: number
  thrust_amperes: number[]
  thrust_power: number[]
  efficiency: number[]
  warmup_time: number
  max_amperes: number
  max_voltage: number
  min_voltage: number
  max_power: number
  max_thrust: number
  data: number[][]
}

export default Vue.extend({
  name: 'DatabasePage',
  async asyncData ({ $axios }) {
    const items = await $axios.$get('/database')

    return { items }
  },
  data () {
    const items: DatabaseItem[] = []

    return {
      items
    }
  }
})
</script>
