<template>
  <div class="flex justify-center">
    <div class="grid grid-cols-2 gap-8">
      <div class="card col-span-2">
        <p>⚙️ Настройки и запуск</p>
        <hr>
        <input v-model="name" class="inp" placeholder="Название" type="text">
        <div class="flex flex-row items-center space-x-2">
          <input v-model="soft_start" type="checkbox">
          <p>Мягкий старт</p>
        </div>
        <hr>
        <button class="btn" @click="startBenchmark">
          Запустить бенчмарк
        </button>
      </div>
      <div class="card">
        <p>⚡ Тяга от тока</p>
        <hr>
        <chart-thrust-amperage :amperes="[1,2,3,4,5,6,7]" :thrust="[12,20,22,23,24,25,67]" />
      </div>
      <div class="card">
        <p>🔌 Тяга от мощности</p>
        <hr>
        <chart-thrust-power :power="[33,77,88,90,92,95,99]" :thrust="[12,20,22,23,24,25,67]" />
      </div>
      <div class="card col-span-2">
        <p>🥇 Эффективность</p>
        <hr>
        <chart-efficiency :efficiency="[0,22,33,44,55,44,33,22,22,21]" :height="200" />
      </div>
      <div class="card col-span-2">
        <p>📉 Результаты</p>
        <hr>
        <p>🔥 Время разгона: 1 сек.</p>
        <div>
          <p>⚡ Макс. ток: 100</p>
          <p>✨ Макс. напряжение: 100</p>
          <p>✨ Мин. напряжение: 100 </p>
          <p>🔌 Макс. мощность: 100</p>
          <p>✈️ Макс. тяга: 100</p>
        </div>
        <table class="mt-4 w-full table-auto">
          <thead>
            <tr class="bg-gray-50">
              <th>_</th>
              <th>T</th>
              <th>P</th>
              <th>I</th>
              <th>V</th>
              <th>t°</th>
            </tr>
          </thead>
          <tbody class="">
            <tr>
              <th>5%</th>
              <th>11</th>
              <th>22</th>
              <th>33</th>
              <th>44</th>
              <th>55</th>
            </tr>
            <tr>
              <th>10%</th>
              <th>11</th>
              <th>22</th>
              <th>33</th>
              <th>44</th>
              <th>55</th>
            </tr>
            <tr>
              <th>15%</th>
              <th>11</th>
              <th>22</th>
              <th>33</th>
              <th>44</th>
              <th>55</th>
            </tr>
            <tr>
              <th>20%</th>
              <th>11</th>
              <th>22</th>
              <th>33</th>
              <th>44</th>
              <th>55</th>
            </tr>
            <tr>
              <th>25%</th>
              <th>11</th>
              <th>22</th>
              <th>33</th>
              <th>44</th>
              <th>55</th>
            </tr>
            <tr>
              <th>30%</th>
              <th>11</th>
              <th>22</th>
              <th>33</th>
              <th>44</th>
              <th>55</th>
            </tr>
          </tbody>
        </table>
      </div>
    </div>
  </div>
</template>

<script lang="ts">
import Vue from 'vue'

export default Vue.extend({
  name: 'BenchmarkPage',
  data () {
    return {
      name: '',
      soft_start: true
    }
  },
  methods: {
    startBenchmark () {
      this.$axios.post('/benchmark', { name: this.name, soft_start: this.soft_start })
    }
  }
})
</script>

<style scoped>

</style>
