<script>
	import "./layout.css"
	let { children } = $props()

	import { Navigation } from "@skeletonlabs/skeleton-svelte"
	import { FingerprintPattern, House, ScanFace, ScrollTextIcon } from "lucide-svelte"
</script>

<svelte:head>
	<title>Dashboard</title>
</svelte:head>

<div class="flex">
	<nav>
		<Navigation class="h-screen" layout="sidebar">
			<Navigation.Content>
				<Navigation.Header></Navigation.Header>
				<Navigation.Menu>
					<Navigation.TriggerAnchor href="/">
						<House />
						<Navigation.TriggerText>Home</Navigation.TriggerText>
					</Navigation.TriggerAnchor>

					<Navigation.TriggerAnchor href="/log">
						<ScrollTextIcon />
						<Navigation.TriggerText>Logs</Navigation.TriggerText>
					</Navigation.TriggerAnchor>
				</Navigation.Menu>
			</Navigation.Content>
		</Navigation>
	</nav>

	<main class="w-full h-dvh overflow-auto">
		{@render children()}
	</main>
</div>
