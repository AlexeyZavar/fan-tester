<script lang="ts">
// eslint-disable-next-line import/named
import { Line, mixins } from 'vue-chartjs'
import Vue from 'vue'

export default Vue.extend({
  name: 'ChartEfficiency',
  extends: Line,
  mixins: [mixins.reactiveProp],
  props: ['efficiency'],
  mounted () {
    // @ts-ignore
    this.renderChart({ labels: [0, 10, 20, 30, 40, 50, 60, 70, 80, 90, 100], datasets: [{ label: 'Эффективность', fill: false, lineTension: 0.1, borderColor: 'black', data: this.efficiency }] })
  }
})
</script>

<style scoped>

</style>
