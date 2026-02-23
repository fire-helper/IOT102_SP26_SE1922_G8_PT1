<script lang="ts">
	import type { PageProps } from "./$types"

	import { Trash } from "lucide-svelte"

	const props: PageProps = $props()
</script>

<div class="m-5">
	<h3 class="h3 mb-5">Registered fingerprints</h3>

	<button onclick={() => fetch("/enroll", { method: "POST" })} class="btn preset-tonal-primary"
		>Enroll new fingerprint</button
	>

	<button onclick={() => fetch("/verify", { method: "POST" })} class="btn preset-tonal-secondary"
		>Verify fingerprint</button
	>

	<table class="table">
		<thead>
			<tr>
				<td>ID</td>
				<td>Enroll time</td>
				<td></td>
			</tr>
		</thead>

		<tbody>
			{#each props.data.fingerprints as fingerprint}
				<tr>
					<td>{fingerprint.id}</td>
					<td>{new Date(fingerprint.registerTimestamp)}</td>
					<td>
						<!--
						<button class="btn preset-tonal-error">
							<Trash />
						</button>
						-->
					</td>
				</tr>
			{/each}
		</tbody>
	</table>
</div>
