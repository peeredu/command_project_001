import { createApp } from 'vue'
import App from './App'
import axios from 'axios'
import router from '@/router/router'
const app = createApp(App)

app.use(router, axios)
app.mount('#app')
