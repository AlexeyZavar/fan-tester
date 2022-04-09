<script lang="ts">
// eslint-disable-next-line import/named
import { Line, mixins } from 'vue-chartjs'
import Vue from 'vue'

export default Vue.extend({
  name: 'ChartThrustAmperage',
  extends: Line,
  mixins: [mixins.reactiveProp],
  mounted () {
    // @ts-ignore
    this.renderChart(this.chartData)
  }
})
</script>

<style scoped>

</style>
