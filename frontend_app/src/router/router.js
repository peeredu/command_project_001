import Home from '@/views/Home'
import Items from '@/views/Items'
import Orders from '@/views/Orders'
import SingleItem from '@/views/SingleItem'
import SingleOrder from '@/views/SingleOrder'
import { createRouter, createWebHistory } from 'vue-router'

const routes = [
    {
        path: '/',
        component: Home,
    },
    {
        path: '/orders',
        component: Orders,
    },
    {
        path: '/items',
        component: Items,
    },
    {
        path: '/items/:id',
        component: SingleItem,
    },
    {
        path: '/orders/:id',
        component: SingleOrder,
    }
]

const router = createRouter({
    routes,
    history: createWebHistory(process.env.BASE_URL)
})

export default router;