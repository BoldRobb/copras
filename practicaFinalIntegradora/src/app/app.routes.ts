import { Routes } from '@angular/router';

import { SawComponent } from './components/saw/saw.component';


export const routes: Routes = [
    {path: '', redirectTo: 'saw', pathMatch: 'full'},

    {path: 'saw', component: SawComponent},

];
