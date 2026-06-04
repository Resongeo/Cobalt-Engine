class Player : IEntity
{
    Entity self;

    Entity ground;

    void Start() {
        ground = Scene::find_by_name("Ground");
    }

    void Update(float dt) {
        ground.set_position_x(1.0);
        self.set_position_x(1.0);
    }
}